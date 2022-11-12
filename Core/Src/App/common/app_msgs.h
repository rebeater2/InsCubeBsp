/**
* @file app_msgs.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/23 13:06
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_COMMON_APP_MSGS_H_
#define INSCUBEBSP_CORE_SRC_APP_COMMON_APP_MSGS_H_
#include "bsp.h"
#include "imu_adis1646x.h"
#include "gnss_mxt906b.h"
typedef struct {
  system_time_t tm;
  imu_raw_adi_t raw;
} imu_msg_t;

typedef struct {
  system_time_t tm;
  gnss_pos_t graw;
} gnss_msg_t;

#endif //INSCUBEBSP_CORE_SRC_APP_COMMON_APP_MSGS_H_
