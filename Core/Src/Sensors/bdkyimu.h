/**
* @file bdkyimu.h in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 10/29/21 10:36 AM
* @version 1.0
**/

#ifndef FUSIONUNITXIH6_CORE_INC_BDKYIMU_H_
#define FUSIONUNITXIH6_CORE_INC_BDKYIMU_H_
#include "spi.h"
typedef struct {
  int16_t temp_low;
  int16_t temp_out;
  int16_t x_gyro_low;
  int16_t x_gyro_out;

  int16_t y_gyro_low;
  int16_t y_gyro_out;
  int16_t z_gyro_low;
  int16_t z_gyro_out;

  int16_t x_acce_low;
  int16_t x_acce_out;
  int16_t y_acce_low;
  int16_t y_acce_out;

  int16_t z_acce_low;
  int16_t z_acce_out;
  uint32_t id;
//  uint16_t reserved;
}ImuRawKy;

int KyImu102A0Initial();
void KyImu102A0ReadRaw(ImuRawKy *raw);



#endif //FUSIONUNITXIH6_CORE_INC_BDKYIMU_H_
