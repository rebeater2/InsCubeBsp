/**
* @file SensorFusionFusion.cpp in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/11/1 0:01
* @version 1.0
**/


#include "SensorFusion.h"
SensorFusionError_t fusion_init(){
    return SensorFusionOk;
};
/**
 *
 * @param timestamp
 * @param data
 * @param sensor
 * @return
 */
SensorFusionError_t fusion_update(double timestamp,const void *data, Sensor_t sensor){
    switch (sensor) {
        case SensorIMU:

            break;
        case SensorGNSS:
            /* TODO: */
            break;
        default:break;
    }
    return SensorFusionOk;
}
SensorFusionError_t fusion_getstatus(FusionStatus_t *status) {
    *status = FusionStatusINS;
    return SensorFusionOk;
}
SensorFusionError_t fusion_getpos(double timestamp, double pos[3]){
    return SensorFusionOk;
}
SensorFusionError_t fusion_geteuler(double timestamp,float[3]){
    return SensorFusionOk;
}