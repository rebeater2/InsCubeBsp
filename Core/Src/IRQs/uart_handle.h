/**
* @file uart_handle.h.cpp in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 12/3/21 5:21 PM
* @version 1.0
**/
#ifndef INS_CUBE_CORE_INC_UART_HANDLE_H_
#define INS_CUBE_CORE_INC_UART_HANDLE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "usart.h"
  /**
   * 串口数据中断用户回调函数
   * @param huart 串口指针
   */
  void USER_UART_IRQHandler(UART_HandleTypeDef *huart);


  /**
   * 通过串口DMA发送数据
   * @param husart  串口句柄
   * @param pdata 数据地址
   * @param length 数据长度
   */
  void SendByUartDma(UART_HandleTypeDef *husart, uint8_t *pdata, uint32_t length);
#ifdef __cplusplus
}
#endif

#endif