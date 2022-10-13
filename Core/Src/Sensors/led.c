//
// Created by rebeater on 3/28/21.
//

#include "led.h"
#include <cmsis_os.h>
#include "../bsp/bsp.h"

void SetLed(LedColor c) {
    switch (c) {
        case LED_OFF: /*BLACK*/


            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_SET);
            break;
        case LED_RED:/*RED*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_RESET);
            break;
        case LED_GREEN:/*GREEN*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_SET);
            break;
        case LED_YELLOW:/*YELLOW*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_RESET);
            break;
        case LED_BLUE:/*BLUE*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_SET);
            break;
        case LED_PINK:/*PINK*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_RESET);
            break;
        case LED_CYAN:/*CYAN 青色*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_SET);
            break;
        case LED_WHITE:/*WHITE*/
            HAL_GPIO_WritePin(RGB_LED_B_GPIO_Port, RGB_LED_B_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_G_GPIO_Port, RGB_LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RGB_LED_R_GPIO_Port, RGB_LED_R_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void LedBlinkTest() {
    for (int i = 0; i < 8; i++) {
	  SetLed((LedColor)i);
        osDelay(1000);
    }
}