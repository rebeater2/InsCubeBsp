//=====================================================================================================
// MahonyAHRS.h
//=====================================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h
#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------------------------------------------------------------------------
// Variable declaration



//---------------------------------------------------------------------------------------------------
// Function declarations
void MahonyAHRSInit(float kp,float ki,float sample_rate);

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

// For output euler angle in ZXY @rebeater
void MahonyAHRSGetEuler(float euler[3]);
#ifdef __cplusplus
}
#endif
#endif
//=====================================================================================================
// End of file
//=====================================================================================================
