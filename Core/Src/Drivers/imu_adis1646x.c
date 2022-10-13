/**
* @file imu_adis1646x.c in InsCube
* @author linfe
* @comment
* Create on 2022/8/28 10:57
* @version 1.0
**/

#include "imu_adis1646x.h"
#include "gpio.h"
#include "main.h"

#define ADI_PI 3.1415926535897932
#ifndef IMU_DEVICE
#define IMU_DEVICE IMU_ADIS16465
#endif

typedef uint16_t adix_reg_t;

adix_attribute_t attrib_list[] = {
    {.name="adis16460",
        .acce_scale = 0.25e-3f,
        .gyro_scale =(float) (0.005 * ADI_PI / 180.0f),
        .max_sample_rate = 2048,
        .id = 0x404C,
        .reg_addr = {
            .MSC_CTRL = 0x32,
            .SYNC_SCAL = 0x34,
            .DEC_RATE = 0x36,
            .FLTR_CTR = 0x38,
            .GLOB_CMD = 0x3E,
            .WHO_AM_I = 0x56,
        }
    },
    {.name="adis16465",
        .acce_scale = 0.25e-3f,
        .gyro_scale =(float) (1.0 / (160.0) * ADI_PI / 180.0),
        .max_sample_rate = 2000,
        .id = 0x4051,
        .reg_addr = {
            .MSC_CTRL = 0x60,
            .SYNC_SCAL = 0x62,
            .DEC_RATE = 0x64,
            .FLTR_CTR = 0x5C,
            .GLOB_CMD = 0x68,
            .WHO_AM_I = 0x72,
        }
    }
};

typedef enum {
#if IMU_DEVICE == IMU_ADIS16465
  /* Register for ADIS16465 */
    adis1646x_MSC_CTRL = 0x60,
    adis1646x_SYNC_SCAL = 0x62,
    adis1646x_DEC_RATE = 0x64,
    adis1646x_FLTR_CTRL = 0x5C,
    adis1646x_GLOB_CMD = 0x68,
    adis1646x_NULL_CNFG = 0x66,
    adis1646x_ADIS1646X_WHO_AM_I = 0x72,
    adis1646x_ADIS1646X_ID = 0x4051,
    adis1646x_BURST_READ_CMD = 0x6800,
    adis1646x_XG_BIAS_LOW = 0x40,
    adis1646x_XG_BIAS_HIGH = 0x42,
    adis1646x_YG_BIAS_LOW = 0x44,
    adis1646x_YG_BIAS_HIGH = 0x46,
    adis1646x_ZG_BIAS_LOW = 0x48,
    adis1646x_ZG_BIAS_HIGH = 0x4A,
    adis1646x_XA_BIAS_LOW = 0x4C,
    adis1646x_XA_BIAS_HIGH = 0x4E,
    adis1646x_YA_BIAS_LOW = 0x50,
    adis1646x_YA_BIAS_HIGH = 0x52,
    adis1646x_ZA_BIAS_LOW = 0x54,
    adis1646x_ZA_BIAS_HIGH = 0x56,
#else
  adis1646x_MSC_CTRL = 0x32,
  adis1646x_SYNC_SCAL = 0x34,
  adis1646x_DEC_RATE = 0x36,
  adis1646x_FLTR_CTR = 0x38,
  adis1646x_GLOB_CMD = 0x3E,
  adis1646x_ADIS1646X_WHO_AM_I = 0x56,

#endif

} adis1646x_reg_t;

#if IMU_DEVICE == IMU_ADIS16465
const double adis1646x_ka_g = 0.25e-3;
/*Kg=160*/
const double adis1646x_kg = 1.0 / (160.0) * PI / 180.0;
#define ADIS1646X_ID 0x4051
#define  BURST_READ_CMD  0x6800
#else
double adis1646x_ka_g = 0.25e-3;
double adis1646x_kg = 0.005 * ADI_PI / 180.0;
#define ADIS1646X_ID 0x404C
#define  BURST_READ_CMD  0x3E00
#endif

static adix_reg_t adis1646x_read_reg(adis1646x_handle_t *dev, uint8_t addr) {
    uint8_t reg[4] = {0};
    reg[1] = addr;
    bsp_io_error_t err = dev_write(&dev->dev, reg, 1);
    dev->delay_ms(1);
    err |= dev_read(&dev->dev, reg, 1);
    if (err) {
        dev->trace("adis_1646x_read failed %d\n", err);
    }
/*	dev->trace("1REG:%X %X %X %X\n", reg[0], reg[1], reg[2], reg[3]);*/
    return (uint16_t) reg[1] << 8u | reg[0];
//	dev->trace("%d %d %d %d\n",dat[0],dat[1],dat[2],dat[3]);
    return (adix_reg_t) reg[1] << 8 | reg[2];
}

static void adis1646x_write_reg(adis1646x_handle_t *handle, uint8_t addr, uint8_t cmd) {
    uint8_t dat[2];
    dat[1] = addr | 0x80;
    dat[0] = cmd;
    dev_write(&handle->dev, dat, 2);
}

static adix_reg_t adis1646x_read_id(adis1646x_handle_t *handle) {

    return adis1646x_read_reg(handle, adis1646x_ADIS1646X_WHO_AM_I);
}

adis1646x_error_t adis1646x_read(adis1646x_handle_t *handle, imu_raw_adi_t *adi) {
    uint8_t cmd[2] = {0, 0};
    cmd[1] = handle->attrib->reg_addr.GLOB_CMD;
    dev_write(&handle->dev, (uint8_t *) &cmd, 1);
//	handle->delay_ms(1);
    dev_read(&handle->dev, (uint8_t *) adi, 10);
    return adis1646x_ok;
}

adis1646x_error_t adis1646x_init(adis1646x_handle_t *handle) {
    adix_reg_t reg;
    uint8_t read_cnt = 0;
    adix_regs_addr_t *addr = NULL;
    if (handle->trace) {
        handle->trace("adis1646x initialize...\n");
    }

    /* step1: detect imu device*/
    int available_dev_size = sizeof(attrib_list) / sizeof(adix_attribute_t);
    do {
        for (int i = 0; i < available_dev_size; ++i) {
            reg = adis1646x_read_reg(handle, attrib_list[i].reg_addr.WHO_AM_I);
//            handle->trace("READ 0X%X = 0X%X\n",attrib_list[i].reg_addr.WHO_AM_I,reg);
            if (reg == attrib_list[i].id) {
                handle->attrib = &attrib_list[i];
                break;
            }
            handle->delay_ms(500);
        }
        read_cnt++;
    } while (!handle->attrib && read_cnt < 5);
    if (read_cnt >= 5) {
        return adis1646x_id_error;
    }
    if (handle->trace) {
        handle->trace("detect imu:%s\n", handle->attrib->name);
        handle->trace("imu self test...\n");
    }

    /* step2: self test */
    addr = &(handle->attrib->reg_addr);
    adis1646x_write_reg(handle, addr->GLOB_CMD, 0x04);
    handle->delay_ms(1000);
    imu_raw_adi_t adi;
    adis1646x_read(handle, &adi);
    if (adi.stat != 0) {
        handle->trace("%s Self-Test FAILED, Stat = %X\n", handle->attrib->name, adi.stat);
        return adis1646x_status_error;
    }
    float temp = 0.05f * (float) adi.temp + 25;
    if (handle->trace) {
        handle->trace("self test pass, IMU:temperature %d deg\n", (int) temp);
    }

    /* step3 configure imu device */
    /*disable sync, set dr to 1 when data ready*/
    adis1646x_write_reg(handle, addr->MSC_CTRL, 0xC1);
    handle->delay_ms(10);
    adis1646x_read_reg(handle, addr->MSC_CTRL);
    handle->delay_ms(10);
    adis1646x_write_reg(handle, addr->DEC_RATE, handle->rate_div);
    handle->deltat = 1.0f / ((float) handle->attrib->max_sample_rate / ((float) handle->rate_div + 1.0f));
    handle->delay_ms(10);
    adis1646x_write_reg(handle, addr->DEC_RATE + 1, 0x00);
    handle->delay_ms(10);
    adis1646x_read_reg(handle, addr->DEC_RATE);
    if (handle->trace){
        handle->trace("imu initialize finished. sample rate=%dHz\n",(int)(1.0/handle->deltat+0.5));
    }
    return adis1646x_ok;
};
adis1646x_handle_t *imu_handle_create() {
    adis1646x_handle_t *handle = bsp_malloc(sizeof(adis1646x_handle_t));
    handle->dev = spi5_dev;
    handle->delay_ms = bsp_delay_ms;
    handle->data_ready_pin = ADI_INT_Pin;
    handle->rate_div = 0x1;
    handle->attrib = NULL;
    return handle;
}

/*
void convert_adi_to_double(ImuRawAdi *adi, ImuData *imu) {
#ifdef RGIOE
	*/
/*RGIOE使用前右下坐标系*//*

  imu->gyro[1] = adis16460_kg * adi->gyro[0];
  imu->gyro[0] = adis16460_kg * adi->gyro[1];
  imu->gyro[2] = -adis16460_kg * adi->gyro[2];
  imu->acce[1] = adis16460_ka_g * adi->acce[0];
  imu->acce[0] = adis16460_ka_g * adi->acce[1];
  imu->acce[2] = -adis16460_ka_g * adi->acce[2];
#else
	imu->gyro[0] = adis16460_kg * adi->gyro[0];
	imu->gyro[1] = adis16460_kg * adi->gyro[1];
	imu->gyro[2] = adis16460_kg * adi->gyro[2];
	imu->acce[0] = adis16460_ka_g * adi->acce[0];
	imu->acce[1] = adis16460_ka_g * adi->acce[1];
	imu->acce[2] = adis16460_ka_g * adi->acce[2];
#endif
}*/
