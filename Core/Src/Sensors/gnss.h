/**
* @file gnss.h in InsCube
* @author rebeater
* @comment
* Create on 4/23/22 8:55 AM
* @version 1.0
**/

#ifndef INSCUBE_CORE_SRC_SENSORS_GNSS_H_
#define INSCUBE_CORE_SRC_SENSORS_GNSS_H_
#include "usart.h"
#include "nmea.h"

typedef enum {
  GnssRxNmea = 0,									/* NMEA in ASCII */
  GnssRxBinary = 1,									/* Binary */
  GnssRxRTCM = 2,									/* RTCM3 */
  GnssRxMix = 3 									/* NMEA and RTCM*/
} GnssRxFormat;
typedef struct {
  UART_HandleTypeDef *huart;                     /* Uart Handle in HAL 不做判空*/
  uint8_t *uart_rx_buff;                         /*Uart receive buffer 严禁为空*/
  uint32_t uart_rx_buff_size;                    /*The length of uart receive buffer,must be Larger than sizeof(LpsRaw)*/
  uint8_t *gnss_buffer;							/*Valid GNSS data buffer*/
  uint32_t valid_gnss_buffer_size;					/*Valid GNSS data size */
  GnssRxFormat rx_format;                        /*GNSS rx format*/
  int (*delay_ms)(uint32_t ms);                 /*延时函数,单位ms*/
  int (*trace)(const char *fmt, ...);            /*日志打印函数,NULL则不输出*/
  void (*error_handler)(void);                	/*错误处理函数,出现严重错误时调用*/
} GnssHandlerDef;

/**
 * @brief GNSS Initialize, Start DMA and Uart Interrupt to receive GNSS data
 * @param dev GNSS Handler
 */
void GnssInitial(GnssHandlerDef *dev);

/**
 * Parse GNSS data from uart buffer
 * @param dev GNSS Handler
 * @param week GNSS week
 * @param ToW GNSS Time of Week
 * @param gnss GNSS position and other  information
 * @return Error Code
 */
GnssErrorCode GnssParseData(GnssHandlerDef *dev, GnssDataRaw *gnss);

/**
 * 复制串口缓冲区的数据,用于其他任务,如存储任务,避免被新中断覆盖
 * @param cnt
 */
void GnssHandleBuff(GnssHandlerDef *dev, uint32_t cnt);
#endif //INSCUBE_CORE_SRC_SENSORS_GNSS_H_
