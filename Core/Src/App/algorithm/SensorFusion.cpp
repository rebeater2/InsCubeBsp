/**
* @file SensorFusionFusion.cpp in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/11/1 0:01
* @version 1.0
**/


#include <cstring>
#include "SensorFusion.h"
#ifdef MAHONY_6_AXIS
#include "Mahony/MahonyAHRS.h"
#endif
#define MAX_FUSION_FREQ 2000
#define MIN_FUSION_FREQ 1
#define MIN_DELTA_T (1.0f/MAX_FUSION_FREQ)
#define MAX_DELTA_T (1.0f/MIN_FUSION_FREQ)

SensorFusionError_t fusion_init(FusionData_t *fd, const FusionOption_t *opt) {
    memset(fd, 0, sizeof(FusionData_t));
    fd->opt = *opt;
#ifdef MAHONY_6_AXIS
    fd->latlng[0] =1.0f;
    fd->latlng[1] =0.123f;
    float deltat = opt->delta_t;
    if (deltat > MAX_DELTA_T) {
        deltat = MAX_DELTA_T;
    }
    if (deltat < MIN_DELTA_T) {
        deltat = MIN_DELTA_T;
    }
    MahonyAHRSInit(1, 0, 1.0f / deltat);
#endif
    return SensorFusionOk;
};
/**
 *
 * @param timestamp
 * @param data
 * @param sensor
 * @return
 */
SensorFusionError_t fusion_update(FusionData_t *fd, const double timestamp, const void *data, Sensor_t sensor) {
    fd->cur_timestamp = timestamp;
    switch (sensor) {
        case SensorIMU: {
            fd->imu = *(ImuData_t *) data;
#ifdef MAHONY_6_AXIS
            MahonyAHRSupdateIMU(fd->imu.gyro[0], fd->imu.gyro[1], fd->imu.gyro[2],
                                fd->imu.acce[0], fd->imu.acce[1], fd->imu.acce[2]
            );
            MahonyAHRSGetEuler(fd->atti);
#endif
        }
            break;
        case SensorGNSS:

            break;
        default:break;
    }
    fd->pre_timestamp = fd->cur_timestamp;
    return SensorFusionOk;
}
SensorFusionError_t fusion_getstatus(FusionData_t *fd, FusionStatus_t *status) {
    *status = FusionStatusINS;
    return SensorFusionOk;
}