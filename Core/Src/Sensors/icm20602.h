//
// Created by rebeater on 2020/9/25.
//

#ifndef LOOSELY_COUPLE_ICM20602_H
#define LOOSELY_COUPLE_ICM20602_H

#include "spi.h"

#define     ICM20602_DEV_ADDR           0x69 //SA0接地：0x68   SA0上拉：0x69  模块默认上拉

#define     ICM20602_SPI_W              0x00
#define     ICM20602_SPI_R              0x80

#define     ICM20602_XG_OFFS_TC_H       0x04
#define     ICM20602_XG_OFFS_TC_L       0x05
#define     ICM20602_YG_OFFS_TC_H       0x07
#define     ICM20602_YG_OFFS_TC_L       0x08
#define     ICM20602_ZG_OFFS_TC_H       0x0A
#define     ICM20602_ZG_OFFS_TC_L       0x0B
#define     ICM20602_SELF_TEST_X_ACCEL  0x0D
#define     ICM20602_SELF_TEST_Y_ACCEL  0x0E
#define     ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define     ICM20602_XG_OFFS_USRH       0x13
#define     ICM20602_XG_OFFS_USRL       0x14
#define     ICM20602_YG_OFFS_USRH       0x15
#define     ICM20602_YG_OFFS_USRL       0x16
#define     ICM20602_ZG_OFFS_USRH       0x17
#define     ICM20602_ZG_OFFS_USRL       0x18
#define     ICM20602_SMPLRT_DIV         0x19
#define     ICM20602_CONFIG             0x1A
#define     ICM20602_GYRO_CONFIG        0x1B
#define     ICM20602_ACCEL_CONFIG       0x1C
#define     ICM20602_ACCEL_CONFIG_2     0x1D
#define     ICM20602_LP_MODE_CFG        0x1E
#define     ICM20602_ACCEL_WOM_X_THR    0x20
#define     ICM20602_ACCEL_WOM_Y_THR    0x21
#define     ICM20602_ACCEL_WOM_Z_THR    0x22
#define     ICM20602_FIFO_EN            0x23
#define     ICM20602_FSYNC_INT          0x36
#define     ICM20602_INT_PIN_CFG        0x37
#define     ICM20602_INT_ENABLE         0x38
#define     ICM20602_FIFO_WM_INT_STATUS 0x39
#define     ICM20602_INT_STATUS         0x3A

#define     ICM20602_ACCEL_XOUT_H       0x3B
#define     ICM20602_ACCEL_XOUT_L       0x3C
#define     ICM20602_ACCEL_YOUT_H       0x3D
#define     ICM20602_ACCEL_YOUT_L       0x3E
#define     ICM20602_ACCEL_ZOUT_H       0x3F
#define     ICM20602_ACCEL_ZOUT_L       0x40
#define     ICM20602_TEMP_OUT_H         0x41
#define     ICM20602_TEMP_OUT_L         0x42
#define     ICM20602_GYRO_XOUT_H        0x43
#define     ICM20602_GYRO_XOUT_L        0x44
#define     ICM20602_GYRO_YOUT_H        0x45
#define     ICM20602_GYRO_YOUT_L        0x46
#define     ICM20602_GYRO_ZOUT_H        0x47
#define     ICM20602_GYRO_ZOUT_L        0x48
#define     ICM20602_SELF_TEST_X_GYRO   0x50
#define     ICM20602_SELF_TEST_Y_GYRO   0x51
#define     ICM20602_SELF_TEST_Z_GYRO   0x52
#define     ICM20602_FIFO_WM_TH1        0x60
#define     ICM20602_FIFO_WM_TH2        0x61
#define     ICM20602_SIGNAL_PATH_RESET  0x68
#define     ICM20602_ACCEL_INTEL_CTRL   0x69
#define     ICM20602_USER_CTRL          0x6A
#define     ICM20602_PWR_MGMT_1         0x6B
#define     ICM20602_PWR_MGMT_2         0x6C
#define     ICM20602_I2C_IF             0x70
#define     ICM20602_FIFO_COUNTH        0x72
#define     ICM20602_FIFO_COUNTL        0x73
#define     ICM20602_FIFO_R_W           0x74
#define     ICM20602_WHO_AM_I           0x75
#define     ICM20602_XA_OFFSET_H        0x77
#define     ICM20602_XA_OFFSET_L        0x78
#define     ICM20602_YA_OFFSET_H        0x7A
#define     ICM20602_YA_OFFSET_L        0x7B
#define     ICM20602_ZA_OFFSET_H        0x7D
#define     ICM20602_ZA_OFFSET_L        0x7E

typedef enum {
  IMU_MODE_CAR = 0, IMU_MODE_PDR
} IMU_MODE;
typedef enum {
  IMU_RATE_200Hz = 0,
  IMU_RATE_100Hz = 1,
  IMU_RATE_50Hz = 2,
  IMU_RATE_10Hz = 3,
  IMU_RATE_1k = 4
} IMU_RATE;
typedef struct {
  uint32_t id;/*区分4个IMU数据*/
  uint32_t PIN_ID;
  int16_t acce[3];/* 2* 3*/
  int16_t temp;/* 2*/
  int16_t gyro[3];/*2 *3*/
  uint16_t RESERVED;
} ImuRawIcm;
extern int16_t icm_gyro_x, icm_gyro_y, icm_gyro_z;
extern int16_t icm_acc_x, icm_acc_y, icm_acc_z;

extern float _accel_scale;
extern float _gyro_scale;
extern double imu_time_local;
extern int flag_imu_update;

#ifdef __cplusplus
#include <iostream>
extern "C" {
#endif


  volatile extern  uint32_t icm20602_id;
//--------硬件SPI--------------
void icm20602_init_spi(IMU_MODE mode, IMU_RATE rate,uint32_t id);

uint16_t icm20602_read_fifo(ImuRawIcm *icm,uint32_t id);

uint8_t icm20602_status(uint32_t id);
#if ENABLE_MULTI_IMUS == 1
void Icm20602MultiInit(IMU_MODE mode, IMU_RATE rate);
#endif
void Icm20602ResetFifo(uint32_t id);
#ifdef __cplusplus
}
#endif

#endif //LOOSELY_COUPLE_ICM20602_H
