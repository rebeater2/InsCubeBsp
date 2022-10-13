//
// Created by rebeater on 6/24/21.
//
#include <string.h>

#include "cmsis_os.h"
#include "usart.h"

uint8_t camera_buffer[128];
extern double system_time_start;
extern int system_time_int;
extern TIM_HandleTypeDef htim2;
extern double pps_margin;
uint8_t flag_camera_event_ready = 0;
#define CAMERA_BUFFER_SIZE 56
/**
 1. @brief Stop the DMA Receive.
 2. @param huart: UART handle.
 3. @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DMAStopRx(UART_HandleTypeDef *huart)
{
  /* Stop UART DMA Rx request if ongoing */
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) &&
	  (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR)))
  {
	CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

	/* Abort the UART DMA Rx channel */
	if(huart->hdmarx != NULL)
	{
	  HAL_DMA_Abort(huart->hdmarx);
	}

	//UART_EndRxTransfer(huart);
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

	/* At end of Rx process, restore huart->RxState to Ready */
	huart->RxState = HAL_UART_STATE_READY;
  }

  return HAL_OK;
}


/*

  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
	cameraEvent.time_s = system_time_start + (system_time_int) + htim2.Instance->CNT * 1.0 / pps_margin;*/
/**//*

	__HAL_UART_CLEAR_IDLEFLAG(huart);
	cameraEvent.camera_info_ = *(CameraInfo*)camera_buffer;
	trace(Error, "Camera CRC check failed %x %s\n", cameraEvent.camera_info_.Header,cameraEvent.camera_info_.ImageName);
*/
/*	crc = crc_checksum((uint8_t *)&(cameraEvent.camera_info_), sizeof(CameraInfo));
//	if (crc != 0) {
	  trace(Error, "Camera CRC check failed %x %s\n", cameraEvent.camera_info_.Header,cameraEvent.camera_info_.ImageName);
//	  return;
	};
	BaseType_t pxHigherPriorityTaskWoken;
	xQueueSendFromISR(cameraDataQueueForStorageHandle, &cameraEvent, &pxHigherPriorityTaskWoken);
	*//*

	huart->RxState = HAL_UART_STATE_READY;
	memset((uint8_t*)&cameraEvent.camera_info_,0,sizeof(CameraInfo));
	HAL_UART_Receive_IT(huart, camera_buffer, sizeof(CameraInfo));
  }
*/

