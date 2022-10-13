//
// Created by rebeater on 5/8/21.
//

#include "stim300.h"
#include <stdio.h>
#include <cmsis_os.h>
#include "stm32h7xx.h"
#include <stm32h7xx_hal_uart.h>

#define CRC32_POLY 0x04C11DB7U
#define CRC32_START 0xFFFFFFFFU
extern UART_HandleTypeDef huart4;
uint8_t rxbuff_stim300[128];
void Stim300UartInit(){
  do {
    HAL_UART_Receive(&huart4, rxbuff_stim300, 1, 0xFFFF);
//    trace(Info, "stim300 : %d\n", rxbuff_stim300[0]);
  } while (rxbuff_stim300[0] != 0x91);
  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  HAL_UART_Receive_IT(&huart4, rxbuff_stim300 + 1, 27);
}
uint32_t stim300_checksum(const uint8_t input_str[28], int num_bytes) {
    uint32_t crc = CRC32_START;
    const uint8_t *p = input_str;
    for (int i = 0; i < num_bytes; i++) {
        crc = crc ^ (*p++ << 24U);
        for (int bit = 0; bit < 8; bit++) {
            if (crc & (1LU << 31U)) crc = (crc << 1U) ^ CRC32_POLY;
            else crc = (crc << 1U);
        }
    }
    return crc;
}

int convert_24bit_to_int(const uint8_t buffer[3]) {
    unsigned int temp = 0x00000000u;
    temp |= (buffer[0] << 24u);
    temp |= (buffer[1] << 16u);
    temp |= (buffer[2] << 8u);
    return (*(int *) (&temp)) >> 8u;
}

int stim300_decode(uint8_t *buff, ImuRawStim *stim) {
    uint8_t *buffer = buff + 1;
    stim->gyro[0] = convert_24bit_to_int(buffer);
    stim->gyro[1] = convert_24bit_to_int(buffer + 3);
    stim->gyro[2] = convert_24bit_to_int(buffer + 6);
    stim->acce[0] = convert_24bit_to_int(buffer + 10);
    stim->acce[1] = convert_24bit_to_int(buffer + 13);
    stim->acce[2] = convert_24bit_to_int(buffer + 16);
//    stim->acce_state = buffer[9];
//    stim->gyro_state = buffer[19];
    return 0;
}


extern uint8_t  rxbuff_stim300[128];
extern double system_time_start;
extern double pps_margin;
extern uint32_t system_time_int;
extern TIM_HandleTypeDef htim2;
extern osMessageQId imuDataQueueHandle;
void uartForStim300Callback(UART_HandleTypeDef *huart){
//    trace(Info,"stim300 ready\n");
    pps_margin = 1e6;
    if (huart->Instance != UART4) return;
    uint32_t crc;
    BaseType_t res, pxHigherPriorityTaskWoken;
    ImuRawStim stim;
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
        stim.time_s = system_time_start + (system_time_int) + htim2.Instance->CNT * 1.0 / pps_margin;/**/
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        crc = stim300_checksum(rxbuff_stim300, 28);
        if (crc != 0) {
//            trace(Error,"CRC check failed\n");
        }
        stim300_decode(rxbuff_stim300, &stim);
        res = xQueueSendFromISR(imuDataQueueHandle, &stim, &pxHigherPriorityTaskWoken);
        huart->RxState = HAL_UART_STATE_READY;
        HAL_UART_Receive_IT(huart, rxbuff_stim300, 28);
    }

}