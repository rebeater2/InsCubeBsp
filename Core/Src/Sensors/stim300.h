/**
* @file stim300.h in InsCube
* @author rebeater
* @comment
* Create on 5/8/21 10:37 PM
* @version 1.0
**/

//
// Created by rebeater on 5/8/21.
//

#ifndef InsCube_STIM300_H
#define InsCube_STIM300_H

#include "usart.h"
typedef struct {
    int gyro[3]; /* 4 * 3 */
    int acce[3]; /* 4 * 3 */
//    uint8_t acce_state;
//    uint8_t gyro_state;
//    uint8_t reserved[30];
    double time_s;
} ImuRawStim;

void Stim300UartInit();
uint32_t stim300_checksum(const uint8_t input_str[28], int num_bytes);
int convert_24bit_to_int(const uint8_t buffer[3]);
int stim300_decode(uint8_t *buff, ImuRawStim *stim);

void uartForStim300Callback(UART_HandleTypeDef *huart);
#endif //InsCube_STIM300_H
