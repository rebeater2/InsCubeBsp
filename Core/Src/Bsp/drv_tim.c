/**
* @file drv_tim.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 19:46
* @version 1.0
**/

#include "bsp.h"
#include "tim.h"

void get_system_time(system_time_t *tm) {
	tm->microsecond = TIM1->CNT;
	tm->millisecond = uwTick;
}
void bsp_tim_init(){
	HAL_TIM_Base_Start(&htim2);
}