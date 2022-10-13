/**
* @file nmea.h in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 12/7/21 1:06 PM
* @version 1.0
**/

#ifndef FUSIONUNITXIH6_CORE_INC_NMEA_H_
#define FUSIONUNITXIH6_CORE_INC_NMEA_H_
#define NMEA_VERSION_3_00 3
#define EMEA_VERSION_4_00 4

#define NMEA_VERSION  NMEA_VERSION_3_00
typedef struct {
  unsigned char year;
  unsigned char mon;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} DateTime;
typedef struct {
  double lat;							/*Latitude in deg*/
  double lon;							/*Longitude in deg*/
  float height; /*32*/					/*Height in deg */
  float pos_std[3];						/*Position error standard deviation in m*/
  float yaw;/*0~360,-1表示无效*/			/*heading angle of double antenna in deg,(0~360) -1:not used*/
  float pitch;							/*pitch angle of double antenna in deg,(0~180) -1:not used*/
  short yaw_std_100;					/*100 times of standard deviation for heading angle */
  short pitch_std_100;					/*100 time of standard deviation for pitch angle*/
  short week;/*  4   */					/*GNSS week*/
  unsigned char ns; 					/*Number of satellites*/
  unsigned char mode; 					/*Position mode,same as NMEA*/
  double gpst;							/*GNSS Time of Week*/
} GnssDataRaw;
typedef enum {
  GNSS_OK = 0,							/*OK*/
  GNSS_NMEA_NOT_FOUND,					/*NMEA Message not found*/
  GNSS_NMEA_NOT_COMPLETE,				/*NMEA Message was found but not complete*/
  GNSS_NMEA_NOT_VALID,					/*NMEA Message was found but the version was NOT supported*/
  GNSS_FORMAT_NOT_SUPPORTED,			/*Current Version does not support this format*/
  GNSS_BIN_CHECK_FAILED					/*Bin message check failed*/
} GnssErrorCode;

extern DateTime utc_time;
/**
 * Decode log message of GNSS devices.
 * @param msg
 * @param gnss
 * @return
 */
GnssErrorCode DecodeMultiMsgs(const char *msg, GnssDataRaw *gnss);
int Nav2GnRmc(double gpst, double lat, double lon, const float *vn, const float *atti, char *buff);
#endif //FUSIONUNITXIH6_CORE_INC_NMEA_H_
