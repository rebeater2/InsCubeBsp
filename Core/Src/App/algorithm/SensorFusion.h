/**
* @file SensorFusionFusion.h in InsCubeBsp
* @author linfe
* @comment
 * SensorFusion means Optimum Estimation for Atti and Position
* Create on 2022/11/1 0:01
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SENSORFUSION_H_
#define INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SENSORFUSION_H_
#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef MAHONY_6_AXIS

#endif

typedef struct {
  float delta_t;
  float acce_scale;
  float gyro_scale;

} FusionOption_t;



typedef enum {
  SensorFusionOk = 0,
} SensorFusionError_t;

typedef enum {
  FusionStatusInit = 0,
  FusionStatusAlign = 1,
  FusionStatusINS = 2,
  FusionStatusFloat = 3,
  FusionStatusFix = 4,
  FusionStatusError = 0xF,
} FusionStatus_t;

typedef enum {
  SensorIMU = 1,
  SensorGNSS = 2,
} Sensor_t;

typedef struct {
  double lat;
  double lon;
  float h;
} GNSSData_t;

typedef struct {
  float acce[3];
  float gyro[3];
} ImuData_t;

typedef struct {
  /* options*/
  FusionOption_t opt;
  double cur_timestamp;
  double pre_timestamp;
  double latlng[2];
  float height;
  float pos_std[3];
  float atti[3];
  float atti_std[3];

  /*sensor data*/
  ImuData_t imu;
  GNSSData_t gnss;
} FusionData_t;


SensorFusionError_t fusion_init(FusionData_t *fd, const FusionOption_t *opt);
/**
 *
 * @param timestamp
 * @param data
 * @param sensor
 * @return
 */
SensorFusionError_t fusion_update(FusionData_t *fd, double timestamp, const void *data, Sensor_t sensor);
SensorFusionError_t fusion_getstatus(FusionData_t *fd, FusionStatus_t *status);

#ifdef __cplusplus
}
#endif

#endif //INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SENSORFUSION_H_
