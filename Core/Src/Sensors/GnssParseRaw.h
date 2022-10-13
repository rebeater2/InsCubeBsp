/**
* @file GnssParseRaw.h in GnssParser
* @author rebeater
* @comment UB482原始数据处理
* Create on 7/27/21 2:42 PM
* @version 1.0
**/

//#include <main.h>
#ifndef GNSSPARSER__GNSSPARSERAW_H_
#define GNSSPARSER__GNSSPARSERAW_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "nmea.h"

typedef enum {
  MSG_Heading = 971,
  MSG_BEST_POS = 42,
  MSG_RANGEA = 41,
} GNSS_MSG_ID;

typedef enum { /*接收机工作状态*/
  GNSS_TYPE_NONE = 0,
  GNSS_TYPE_FIXEDPOS = 1,
  GNSS_TYPE_FIXDHEIGHT = 2,
  GNSS_TYPE_DOPPLER_VELOCITY = 8,/*当前速度是多普勒输出值*/
  GNSS_TYPE_SINGLE = 16,/*单点定位*/
  GNSS_TYPE_PSRDIFF = 17, /*伪距差分*/
  GNSS_TYPE_WAAS = 18,/*SBAS 定位*/
  GNSS_TYPE_L1_FLOAT = 32,/*L1 浮点解*/
  GNSS_TYPE_IONOFREE_FLOAT = 33,/*消去电离层浮点解*/
  GNSS_TYPE_NARROW_FLOAT = 34,/*窄项浮点解*/
  GNSS_TYPE_L1_INT = 48,/*L1 固定解*/
  GNSS_TYPE_WIDE_INT = 49,/*宽巷固定解*/
  GNSS_TYPE_NARROW_INT = 50,/*窄巷固定解*/
  GNSS_TYPE_INS = 52,/*纯惯导*/
  GNSS_TYPE_INS_PSRSP = 53,/*惯导与伪距单点定位*/
  GNSS_TYPE_INS_PSRDIFF = 54,/*惯导与伪距差分定位*/
  GNSS_TYPE_INS_RTKFLOA = 55,/*惯导与载波相位差分浮点解组合*/
  GNSS_TYPE_INS_RTKFIXED = 56,/*惯导与载波相位差分固定解组合*/
} GnssType;
typedef enum {
  SOL_COMPUTED = 0,
  INSUFFICIENT_OBS = 1,/*观测数据不足*/
  NO_CONVERGENCE = 2,/*无法收敛*/
  COV_TRACE = 4,/*协方差矩阵的迹超过最大值1000m*/
} SolutionStatus;


typedef struct {
  uint8_t sync[3];
  uint8_t HeaderLength;
  uint16_t MessageID;
  uint8_t MessageType;
  uint8_t Reserved0;
  uint16_t MessageLength;
  uint16_t Reserved1;
  unsigned char IdleTime;
  unsigned char TimeStatus;
  uint16_t Week;
  uint32_t Second;/*GPS second*/
  uint32_t Reserved2;
  uint16_t Bd2TimeOffsetToGPSSecond;
  uint16_t Reserved3;
} RawHeader;

typedef struct {
  uint32_t SolStatus;
  uint32_t PosType;
  double Lat;
  double Lon;
  double Hgt;
  float Undulation;
  uint32_t DatumId;/*default WGS84=61*/
  float LatStd;
  float LonStd;
  float HgtStd;
  char StnId[4];
  float DiffAge;/*差分龄期*/
  float SolAge;/*定位龄期*/
  uint8_t Svs;
  uint8_t SolnSvs;/*参与计算的卫星数量*/
  uint8_t Reserved[3];
  uint8_t ExtSolSta;
  uint8_t Reserved1;
  uint8_t SigMask;
  uint32_t CrcByte;
} BestPos;
typedef struct {
  uint32_t SolStatus;
  uint32_t PosType;
  float Length;
  float Heading;
  float Pitch;
  float Hdgstddev;
  float PtchStdDev;
  char StnId[4];
  uint8_t Svs;
  uint8_t SolnSvs;
  uint8_t Obs;
  uint8_t Multi;
  uint8_t Reserved0;
  uint8_t ExtSolStat;
  uint8_t Reserved1;
  uint8_t SigMask;
  uint32_t CrcByte;
  uint32_t Reserve1;
} GnssHeading;

//int ParseRawBin(const uint8_t *data, GnssDataRaw *gnss);

#ifdef __cplusplus
}
#endif
#endif //GNSSPARSER__GNSSPARSERAW_H_
