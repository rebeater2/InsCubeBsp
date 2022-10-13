/**
* @file imu_adis1646x.h in InsCube
* @author linfe
* @comment
* Create on 2022/8/28 10:58
* @version 1.0
**/

#ifndef INSCUBE_CORE_SRC_SENSORS_IMU_ADIS1646X_H_
#define INSCUBE_CORE_SRC_SENSORS_IMU_ADIS1646X_H_
#include "../Bsp/bsp.h"

#define IMU_ADIS16460 0
#define IMU_ADIS16465 1

#define IMU_DEVICE IMU_ADIS16460

typedef struct {
  uint8_t MSC_CTRL;
  uint8_t SYNC_SCAL;
  uint8_t DEC_RATE;
  uint8_t FLTR_CTR;
  uint8_t GLOB_CMD;
  uint8_t WHO_AM_I;
} adix_regs_addr_t;

typedef struct {
  float acce_scale;
  float gyro_scale;
  char name[16];
  uint32_t id;
  adix_regs_addr_t reg_addr;
  uint32_t max_sample_rate;
} adix_attribute_t;

typedef struct {
  uint16_t stat;/*STAT_OUT*/
  int16_t gyro[3];
  int16_t acce[3];
  int16_t temp;/*TEMP_OUT*/
  uint16_t cnt;/*SMPL_CNTR*/
  uint16_t checksum;
  uint32_t id;
} imu_raw_adi_t; /*size of ADI=2+2*3+2*3+2+2+2+8 =32 bytes*/


typedef struct {
  bsp_device_t dev;                             /* bsp_device*/
  void (*delay_ms)(uint32_t ms);                /* 延时函数,单位ms*/
  int (*trace)(const char *fmt, ...);           /* 打印函数*/
  uint16_t data_ready_pin;                         /* data ready 中断引脚*/
  uint16_t rate_div;                             /* for ADIS16460: d_rate = 2028 / (rate_div+1)*/
                                                /* for ADIS16465: d_rate = 2000 / (rate_div+1)*/
  float deltat;
  adix_attribute_t *attrib;                      /* pointer to imu attribute struct*/

} adis1646x_handle_t;

typedef enum {
  adis1646x_ok,
  adis1646x_id_error,
  adis1646x_status_error,
} adis1646x_error_t;

adis1646x_handle_t *imu_handle_create();
adis1646x_error_t adis1646x_init(adis1646x_handle_t *handle);
adis1646x_error_t adis1646x_read(adis1646x_handle_t *handle, imu_raw_adi_t *adi);

extern double adis1646x_ka_g;
extern double adis1646x_kg;

#endif //INSCUBE_CORE_SRC_SENSORS_IMU_ADIS1646X_H_
