/**
* @file gnss.c in InsCube
* @author rebeater
* @comment
* Create on 4/23/22 8:55 AM
* @version 1.0
**/

#include "gnss.h"
#include "nmea.h"
#include <memory.h>  /* for memcpy*/
void GnssInitial(GnssHandlerDef *dev) {
  if (dev->huart) {
	__HAL_UART_ENABLE_IT(dev->huart, UART_IT_IDLE);
	HAL_StatusTypeDef status = HAL_UART_Receive_DMA(dev->huart, dev->uart_rx_buff, dev->uart_rx_buff_size);
	if (status != HAL_OK && dev->trace) {
	  if (dev->trace)
		dev->trace("DMA receive failed\n");
	  if (dev->error_handler)
		dev->error_handler();
	}
  }
}
/**
 * 从指定的内存区域找到$G开头的数据,视为找到NMEA消息起始地址
 * @param start 缓存区起始位置
 * @param end 缓存区结束位置
 * @return 如果找到了,返回第一个指向$指针,否则返回NULL
 */
char *FindNmea(const uint8_t *start, const uint8_t *end) {
  uint8_t *p = (uint8_t *)start;
  while (p != end) {
	if ((*p == '$') && (*(p + 1) == 'G')) {
	  return (char *)p;
	}
	p++;
  }
  return NULL;
}

GnssErrorCode GnssParseData(GnssHandlerDef *dev, GnssDataRaw *gnss) {
  if (dev->rx_format == GnssRxMix) {
    /*查找NMEA消息*/
	char *p = FindNmea(dev->uart_rx_buff, dev->uart_rx_buff + dev->uart_rx_buff_size);
	if (p) {
	  /*找到之后解析NMEA消息*/
	  return DecodeMultiMsgs(p, gnss);
	}else{
	  /*没找到返回错误*/
	  return GNSS_NMEA_NOT_FOUND;
	}
  } else {
    /*暂时不解析其他二进制,其实是兼容北斗星通的*/
	dev->trace("Error,current format %d is not support", dev->rx_format);
	if (dev->error_handler)
	  dev->error_handler();
	return GNSS_FORMAT_NOT_SUPPORTED;
  }

}
void GnssHandleBuff(GnssHandlerDef *dev, uint32_t cnt){
  dev->valid_gnss_buffer_size = cnt;
  memcpy(dev->gnss_buffer, dev->uart_rx_buff, cnt);
};
