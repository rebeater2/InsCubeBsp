/**
* @file led.h in loosely_couple_h7
* @author rebeater
* @comment 设置LED颜色
* Create on 3/28/21 11:33 AM
* @version 1.0
**/

//
// Created by rebeater on 3/28/21.
//

#ifndef INS_CUBE_LED_H
#define INS_CUBE_LED_H
#include "main.h"

typedef enum  {
    LED_OFF = 0,
    LED_RED = 1,
    LED_GREEN = 2,
    LED_YELLOW = 3,
    LED_BLUE = 4,
    LED_PINK = 5,
    LED_CYAN = 6,
    LED_WHITE = 7
}LedColor;

void SetLed(LedColor c);
void LedBlinkTest();

#endif //INS_CUBE_LED_H
