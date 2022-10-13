/**
* @file lps.c in InsCube
* @author rebeater
* @comment
* Create on 3/23/22 3:22 PM
* @version 1.0
**/
#include "lps.h"
const unsigned int LPS_HEADER = 0XAA55AA55;
const unsigned int LPS_ENDMARK = 0X0C3C0C3C;
const unsigned int CRC32_POLY = 0x04C11DB7U;
const unsigned int CRC32_START = 0xFFFFFFFFU;
uint32_t LpsCheckSum(uint8_t *indata, size_t num_bytes) {
  /*软件校验*/
  uint32_t crc = CRC32_START;
  const uint8_t *p = indata;
  for (int i = 0; i < num_bytes; i++) {
	crc = crc ^ (*p++ << 24U);
	for (int bit = 0; bit < 8; bit++) {
	  if (crc & (1LU << 31U)) crc = (crc << 1U) ^ CRC32_POLY;
	  else crc = (crc << 1U);
	}
  }
  return crc;
}
void LpsInitialize(LpsHandlerDef *dev) {
  /*开启串口中断*/
  if (dev->huart) {
	HAL_NVIC_EnableIRQ(UART4_IRQn);
	__HAL_UART_ENABLE_IT(dev->huart, UART_IT_IDLE);
	HAL_StatusTypeDef status = HAL_UART_Receive_DMA(dev->huart, dev->uart_rx_buff, dev->uart_rx_buff_size);
	if (status != HAL_OK && dev->trace) {
	  if (dev->trace)
		dev->trace("DMA receive failed,ERROR=%d\n",dev->huart->ErrorCode);
	  if (dev->error_handler)
		dev->error_handler();
	}
  }
};
LpsErrorCode LpsGetRawData(LpsHandlerDef *dev, LpsRaw *raw) {
  if (*(uint32_t *)dev->uart_rx_buff == LPS_HEADER) {
	if (LpsCheckSum(dev->uart_rx_buff, sizeof(LpsRaw)) == 0) {
	  *raw = *(LpsRaw *)dev->uart_rx_buff;
	  return LPS_OK;
	}
  } else {
	return LPS_CHECK_ERROR;
  }
  return LPS_CHECK_ERROR;
}
