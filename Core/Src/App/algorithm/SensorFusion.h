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

SensorFusionError_t fusion_init();
/**
 *
 * @param timestamp
 * @param data
 * @param sensor
 * @return
 */
SensorFusionError_t fusion_update(double timestamp, const void *data, Sensor_t sensor);
SensorFusionError_t fusion_getpos(double timestamp, double pos[3],float std[3]);
SensorFusionError_t fusion_geteuler(double timestamp, float[3],float std[3]);
SensorFusionError_t fusion_getstatus(FusionStatus_t *status);

#ifdef __cplusplus
}
#endif

#endif //INSCUBEBSP_CORE_SRC_APP_ALGORITHM_SENSORFUSION_H_
