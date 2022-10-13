/**
* @file nmea.c in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 12/7/21 1:06 PM
* @version 1.0
**/


#include <global_defines.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "nmea.h"

uint32_t GPST_UTC0 = 18;/*闰秒*/
const double _knot = (0.5144444);

int gps_current_week;
double gpst_start_of_day;/* GPST - UTC0 = 18 全局变量，用于解算GGA消息*/
double utc_pre;
char *headers[4] = {"$GNGGA", "$GNRMC", "$GNGST", "$GPGST"};
uint8_t flags[4] = {0, 0, 0};
DateTime utc_time;
/**
 * ddmm.mmmm -> deg
 * @param ddmmmm
 * @return
 */
inline double DdmmmmToDeg(double ddmmmm) {
  int dd = (int)(ddmmmm / 100);
  return (dd + ((ddmmmm) - 100 * dd) / 60);
}
/**
 * convert deg(in double) to ddmm.mmmm or dddmm.mmmm;
 */
inline double DegToDmm(double deg) {
  int dd = (int)deg;
  return dd * 100 + 60 * (deg - dd);
}

#define RESET_FLAG(flags) (flags)[0]=(flags)[1]=(flags)[2]=0

/*leap year*/
int is_leap_year(int year) {
  if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
    return 1;
  } else {
    return 0;
  }
}
/**
 * 计算当前日期到1970年1月6日所经过的天数。
 * @param year : 2017
 * @param mon : 4
 * @param day :12
 * @return
 */
int day_from_gps0(const int year, const int mon, const int day) {
  int32_t months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int32_t months_leap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int32_t days = -6;
  for (int y = 1980; y < year; y++) {
    days += (365 + is_leap_year(y));
  }
  if (is_leap_year(year)) {
    for (int m = 0; m < mon - 1; m++) {
      days += months_leap[m];
    }
  } else {
    for (int m = 0; m < mon - 1; m++) {
      days += months[m];
    }
  }
  days += day;
  return days;
}

/*GPS时间转换为UTC时间*/
void gpst_to_utc(int week, double gpst, DateTime *time) {
  unsigned short months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  unsigned short months_leap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int days = week * 7 + (int)(gpst / 86400.0);
  time->year = (int)(days / 365);
  int doy = days - day_from_gps0(time->year + 1980, 0, 0);
  if (doy < 0) {
    time->year--;
    doy = days - day_from_gps0(time->year + 1980, 0, 0);
  }
  unsigned short *p_month;
  if (is_leap_year(time->year + 1980)) {
    p_month = months_leap;
  } else {
    p_month = months;
  }
  int i;
  for (i = 0; i < 12; i++) {
    doy -= p_month[i];
    if (doy < 0) {
      break;
    }
  }
  time->mon = i + 1;
  time->day = doy + p_month[i];

  /*算时间*/
  double sod = gpst - 86400 * (int)(gpst / 86400);
  time->hour = (int)(sod / 3600);
  sod -= (3600 * time->hour);
  time->minute = (int)((sod) / 60);
  sod -= (60 * time->minute);
  time->second = (int)sod;
}



uint8_t check(const char *msg) {
  const char *p = msg;
  uint16_t sum = 0;
  while (*p != '*') {
	sum += *p;
	p++;
  }
  char buffer[3];
  sprintf(buffer, "%02X", sum);
  return buffer[0] == (*(++p)) && buffer[1] == (*(++p));
}

/**
 * 解码GNGGA/GPGGA消息
 * @param msg GGA消息 eg:"$GNGGA,110221.000,3152.971754,N,11102.555055,E,1,25,0.72,633.015,M,0,M,,*57␍␊"
 * @param gnss
 * @return
 */
const char *decode_gga(const char *msg, GnssDataRaw *gnss) {
  int hour, minute;
  double second;
  const char *p = strchr(msg, ',');/*跳过GPGGA*/
  char *end = NULL;
  double utc0 = strtod(++p, &end);/*110221.000*/
  if (check(p)) return NULL;
  if (utc0 > utc_pre) {
	RESET_FLAG(flags);
	utc_pre = utc0;
  }
  hour = (int)utc0 / 10000;
  minute = (int)(utc0 / 100) % 100;
  second = utc0 - hour * 10000 - minute * 100;
  utc_time.hour = hour;
  utc_time.minute = minute;
  utc_time.second = (uint8_t)second;
  gnss->week = (short )gps_current_week;
  gnss->gpst = gpst_start_of_day + hour * 3600 + minute * 60 + second;
  p = end;
  gnss->lat = (strtod(++p, &end));
  gnss->lat = DdmmmmToDeg(gnss->lat);
  p = end;
  if (*(++p) == 'S')
	gnss->lat *= -1;
  p++;
  gnss->lon = (strtod(++p, &end));
  gnss->lon = DdmmmmToDeg(gnss->lon);
  p = end;
  if (*(++p) == 'W') {
	gnss->lon *= -1;
  }
  p++;
  gnss->mode = (int)strtol(++p, &end, 10);
  p = end;
  gnss->ns = (int)strtol(++p, &end, 10);
  p = end;
  strtof(++p, &end);/*hdop is neglected*/
  p = end;
  float alt = strtof(++p, &end);
  p = end;
  if (*(++p) != 'M') {
	/*do something to change unit to M*/
	p = (void *)p;
  }
  p++;
  //    printf("strtod(++p, %f\n",strtod(++p, &end));
  gnss->height = alt + strtof(++p, &end);
  return p;
}

/**
 * 解析rmc消息，得到utc时间，主要是日期
 * @param msg
 * @param gnss
 * @return
 */
const char *decode_rmc(const char *msg, GnssDataRaw *gnss) {
  const char *p = msg;
  char *end = NULL;
  int i = 0, year, mon, day, days, utc_date, hour, minute;
  double second;
  p = strchr(p, ',');
  double utc0 = strtod(++p, &end);
  hour = (int)utc0 / 10000;
  minute = (int)(utc0 / 100) % 100;
  second = utc0 - hour * 10000 - minute * 100;
  if (utc0 > utc_pre) {
	RESET_FLAG(flags);
	utc_pre = utc0;
  }

  p = end;
  int skip;
#if NMEA_VERSION == NMEA_VERSION_4_00
  skip = 9;
#else
  skip = 8;
#endif
  while (i < skip) {
	p = strchr(p, ',');
	p++;/*UTC时间*/
	i++;
  }
  utc_date = (int)strtol(p, &end, 10);/*160221*/
  year = 2000 + utc_date % 100;
  mon = (utc_date / 100) % 100;
  day = utc_date / 10000;
  utc_time.year = utc_date % 100 + 20;
  utc_time.mon = mon;
  utc_time.day = day;
  days = day_from_gps0(year, mon, day);
  gps_current_week = (days - days % 7) / 7;
  gpst_start_of_day = (days - 7 * gps_current_week) * 86400 + GPST_UTC0;/* GPST - UTC0 = 18 全局变量，用于解算GGA消息*/
  gnss->week =(short) gps_current_week;
  gnss->gpst = gpst_start_of_day + hour * 3600 + minute * 60 + second;
  /*    printf("gpst_start of day=%f \n", gpst_start_of_day);
	  printf("week=%d \n", week);*/
  return p;
}

const char *offset_msg(const char *p, uint32_t cnt) {
  for (int i = 0; i < cnt; i++) {
	p = strchr(p, ',');
	p++;
  }
  return p;
}

/**
 * 解析HEADINGA语句
 * @param msg #HEADINGA,COM1,0,79.0,FINE,2162,182234.000,1736685,29,18;SOL_COMPUTED,NARROW_FLOAT,0.9558,12.6412,-25.2092,0.0000,186.0738,365.1245,"999",16,11,11,2,3,00,1,71*3252f733
 * @param gnss
 * @return
 */
const char *decode_headinga(const char *msg, GnssDataRaw *gnss) {
  const char *p = msg;
  char *end = NULL;
  p = offset_msg(p, 4);
  if (strncmp(p, "FINE", 4) != 0) {
	gnss->yaw = -1;
	gnss->yaw_std_100 = 10000;
	return p;
  }
  p = offset_msg(p + 1, 5);
  GPST_UTC0 = strtol(p, &end, 10);
  p = offset_msg(p + 1, 1);
  if (strncmp(p, "NARROW", 5) == 0) {
	p = offset_msg(p + 1, 2);
	gnss->yaw = strtof(p, &end);
	p = end + 1;
	gnss->pitch = strtof(p, &end);
	p = end + 1;
	p = offset_msg(p + 1, 1);
	gnss->yaw_std_100 =(short )(100* strtof(p, &end));
	p = end + 1;
	gnss->pitch_std_100 =(short ) (100 * strtof(p, &end));
  } else {
	gnss->yaw = -1;
	gnss->yaw_std_100 = 10000L;
	return p;
  }
  return 0;
}

/**
 *  "$GNGSA,085656.00,1.15,1.12,0.92,7.2954,1.11,0.93,2.53*7F\n"
 * @param msg
 * @return
 */
const char *decode_gst(const char *msg, GnssDataRaw *gnss) {
  const char *p = msg;
  char *end = NULL;
  int i = 0;
  p = strchr(p, ',');
  double utc0 = strtod(p, &end);
  if (utc0 > utc_pre) {
	RESET_FLAG(flags);
	utc_pre = utc0;
  }
  p = end;
  while (i < 6) {
	p = strchr(p, ',');
	if (p == NULL) {
	  return NULL;
	}
	p++;/*UTC时间*/
	i++;
  }
  gnss->pos_std[0] = strtof(p, &end);
  p = end;
  gnss->pos_std[1] = strtof(++p, &end);
  p = end;
  gnss->pos_std[2] = strtof(++p, &end);
  return p;
}

const char *decode_single(const char *msg, GnssDataRaw *gnss, int idx) {
  switch (idx) {
	case 0: return decode_gga(msg, gnss);
	case 1: return decode_rmc(msg, gnss);
	case 2:
	case 3: return decode_gst(msg, gnss);
	default: return NULL;
  }
}

int DecodeSingleMsg(const char *msg, GnssDataRaw *gnss) {
  const char *p = msg;
  GnssErrorCode res;
  int idx;
  for (idx = 0; idx < 4; idx++) {
	if (!strncmp(p, headers[idx], 6)) {
	  p = decode_single(p, gnss, idx);
	  if (p == NULL) return GNSS_NMEA_NOT_COMPLETE;
	  flags[idx] = 1;
	  return GNSS_NMEA_NOT_COMPLETE;
	}
  }
  if (*p == '#') {
	if (!strncmp(p, "#HEADINGA", 8) && (p[9] == ',')) {/*还有个"#HEADINGA2"*/
	  decode_headinga(p, gnss);
	  return GNSS_OK;
	}
  }
  return p - msg;
}
/**
 * Decode GNSS messages to GNSSData
 * @param msg
 * @param gnss
 * @return valid length of message
 */
GnssErrorCode DecodeMultiMsgs(const char *msg, GnssDataRaw *gnss) {
  const char *p = msg;
  while (*p != 0) {
	if (*p == '$')
	  for (int idx = 0; idx < 4; idx++) {
		if (!strncmp(p, headers[idx], 6)) {
		  p = decode_single(p, gnss, idx);
		  if (p == NULL) return GNSS_NMEA_NOT_COMPLETE;
		  flags[idx] = 1;
		}
	  }
#if ENABLE_DOUBLE_ANTENNA == 1
	if (*p == '#') {
		if (!strncmp(p, "#HEADINGA", 8) && (p[9] == ','))
		  p = decode_headinga(p, gnss);
	  }
#endif
	p++;
  }
  /*  if (!strncmp(p, "#HEADINGA", 8) != NULL) {
	  printf("headinga detected\n");
	}*/
  /*    if (!flags[1]) {
		  log_INFO("(%.0f)NO RMC Message!!\n", gnss->data.gpst);
	  }*/
  return GNSS_OK;/*xia*/
}

void UpdateDateTime(double gpst) {
  double sod = gpst - gpst_start_of_day;
  utc_time.hour = (int)(sod / 3600);
  sod -= (3600 * utc_time.hour);
  utc_time.minute = (int)((sod) / 60);
  sod -= (60 * utc_time.minute);
  utc_time.second = (int)sod;
}

/**
 * convert GPST to $GPRMC
 * $GPRMC, <1>, <2>, <3>, <4>, <5>, <6>, <7>, <8>, <9>, <10>, <11>, <12>*hh
 * @param nav
 * @param buff
 * @return
 */
int Nav2GnRmc( double gpst,double lat,double lon,const float *vn,const float *atti, char *buff) {
  UpdateDateTime(gpst);
  char mode = 'A';
  char latNS = 'N';

  if (lat < 0) {
	latNS = 'W';
	lat = -lat;
  }
  char lonEW = 'E';
  if (lon < 0) {
	lonEW = 'W';
	lon = -lon;
  }
  lat = DegToDmm(lat);
  lon = DegToDmm(lon);
  float v = sqrt(vn[0] * vn[0] + vn[1] * vn[1] +vn[2] * vn[2]) / (float)_knot;
  float yaw = atti[2]>0?atti[2]:360+atti[2];
  int tail = sprintf(buff, "$GPRMC,%02d%02d%02d.00,A,%09.4f,%c,%09.04f,%c,%.3f,%.1f,%02d%02d%02d,,,%c*",
					 utc_time.hour, utc_time.minute, (int)utc_time.second,
					 lat, latNS, lon, lonEW, v,yaw,utc_time.day,utc_time.mon,utc_time.year-20,mode
  );
  if (tail < 0) return tail;
  uint8_t checksum = 0;
  for (int i = 1; i < tail-1; ++i) {
	checksum ^= buff[i];
  }
  return tail + sprintf(buff + tail, "%02X\r\n", checksum);
};
