/**
* @file defines.h in loosely_couple_h7
* @author rebeater
* @comment
* Create on 7/19/21 2:23 PM
* @version 1.0
**/


#ifndef INSCUBE_CORE_INC_GLOBAL_DEFINES_H_
#define INSCUBE_CORE_INC_GLOBAL_DEFINES_H_
/** WARNING: Do NOT change it!!!**/
#define GNSS_RCV_UB482 0
#define GNSS_RCV_MXT906B 1

#define IMU_ICM20602 0
#define IMU_ADIS16460 1
#define IMU_STIM300 2
#define IMU_KYIMU102 3
/*ADIS16465 支持*/
#define IMU_ADIS16465 4


#ifndef IMU_DEVICE
#define IMU_DEVICE IMU_ADIS16465 /* IMU Choose*/
#endif
#ifndef ENABLE_MULTI_IMUS
#define ENABLE_MULTI_IMUS 0
#if ENABLE_MULTI_IMUS && (IMU_DEVICE!=IMU_ICM20602)
/*多IMU必须在IMU设备是ICM20602的启用，其他IMU板子上最多只有1个*/
#error "if you want to set ENABLE_MULTI_IMUS to 1,you must set IMU_DEVICE to IMU_ICM20602 "
#endif
#endif
#define ENABLE_LPS 1
#if ENABLE_LPS == 1
#define LPS_TX_LENGTH  44
#endif
#ifndef GNSS_RCV
/*指定接收机型号*/
#define GNSS_RCV GNSS_RCV_MXT906B
#endif


/*对准模式选择 */
#if GNSS_RCV == GNSS_RCV_UB482
#ifndef ENABLE_DOUBLE_ANTENNA
#define ENABLE_DOUBLE_ANTENNA 0
#endif
#else
#define ENABLE_DOUBLE_ANTENNA 0
#endif

#ifndef USE_STIM300
#define USE_STIM300 0
#endif

#ifndef DEBUG_INDOOR
#define DEBUG_INDOOR 0  /*GNSS室内测试*/
#endif
#ifndef RUN_IN_STM32
#define RUN_IN_STM32 1
#endif
/*#if IMU_DEVICE==IMU_ADIS16460
#define IMU_RAW ImuRawAdi
#elif IMU_DEVICE == IMU_KYIMU102
#define IMU_RAW ImuRawKy
#elif IMU_DEVICE == IMU_ICM20602
#define IMU_RAW ImuRawIcm
#elif IMU_DEVICE == IMU_STIM300
#define IMU_RAW ImuRawStim
#elif
#error("IMU Device NOT SUPPORTED!")
#endif*/
#define ALIGN_RAM_D1 __attribute__((section (".ram_d1")))
#endif //INSCUBE_CORE_INC_GLOBAL_DEFINES_H_
