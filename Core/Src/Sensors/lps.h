/**
* @file lps.c in InsCube
* @author rebeater
* @comment
* Create on 3/23/22 3:19 PM
* @version 1.0
**/
#include "main.h"
typedef enum {
  LPS_FIX = 4,
  LPS_INVALID = 0,
} LpsMode;
typedef struct {
  uint32_t header;
  uint32_t week;
  double gpst;
  double lat;
  double lon;
  float height;
  short yaw;
  short pitch;
  uint16_t mode;
  uint16_t ns;
  uint16_t yaw_std;
  uint16_t pitch_std;
  uint16_t pos_std[3];
  uint16_t reserved;
  uint32_t tail;
  uint32_t check;
} LpsRaw;
typedef struct {
  uint32_t header;
  uint32_t week;
  double gpst;
  double lat;
  double lon;
  float height;
  uint32_t info;
  uint32_t endmark;
  uint32_t crc;
} LpsTxPackage;

typedef enum {
  LPS_OK = 0,
  LPS_CHECK_ERROR,                                /*CRC check failed*/
} LpsErrorCode;

typedef struct {
  UART_HandleTypeDef *huart;                     /* Uart Handle in HAL 不做判空*/
  uint8_t *uart_rx_buff;                         /*Uart receive buffer 严禁为空*/
  uint32_t uart_rx_buff_size;                    /*The length of uart receive buffer,Larger than sizeof(LpsRaw)*/
  int (*delay_ms)(uint32_t ms);                 /*延时函数,单位ms*/
  int (*trace)(const char *fmt, ...);            /*日志打印函数,NULL则不输出*/
  void (*error_handler)(void);                	/*错误处理函数,出现严重错误时调用,可以为NULL*/
} LpsHandlerDef;

/**
 * @brief start UART4 RX DMA for LPS
 * @param dev LPS handle
 */
void LpsInitialize(LpsHandlerDef *dev);

/**
 * @brief parse LPS  data from uart buffer
 * @param dev: pointer to LpsHandle
 * @param buffer: pointer to buffer
 * @param raw: pointer to LpsRaw,
 * @return Error Code
 */
LpsErrorCode LpsGetRawData(LpsHandlerDef *dev, LpsRaw *raw);

