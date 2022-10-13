/**
* @file uart_handle.c in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 12/3/21 5:22 PM
* @version 1.0
**/
#include <main.h>
#include <memory.h>
#include <usart.h>
#include "../Sensors/lps.h"
#include <global_defines.h>

void UartDmaRxStop(UART_HandleTypeDef *huart) {
  __HAL_UART_CLEAR_IDLEFLAG(huart);
  HAL_DMA_Abort(huart->hdmarx);
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_EIE | USART_CR3_RXFTIE));
  huart->RxState = HAL_UART_STATE_READY;
  huart->RxISR = NULL;
}/* At end of Tx process, restore huart->gState to Ready */


void USER_UART_IRQHandler(UART_HandleTypeDef *huart) {
}

void SendByUartDma(UART_HandleTypeDef *husart, uint8_t *pdata, uint32_t length) {
  SCB_CleanInvalidateDCache_by_Addr((uint32_t *)pdata, 32 * (uint32_t)(length / 32U + 1));
  HAL_UART_Transmit_DMA(husart, (uint8_t *)pdata, length);
}
