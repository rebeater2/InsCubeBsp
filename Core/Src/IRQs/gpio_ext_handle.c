/**
* @file gpio_ext_handle.cpp in InsCube
* @author rebeater
* @comment
* Create on 3/23/22 8:29 PM
* @version 1.0
**/

#include "../App/tasks/user_tasks.h"
#include "tim.h"
#include "global_defines.h"
extern osMessageQId msgIcmInterruptQueueHandle;
              /*两个PPS之间的间隔，用于修正本地时间 */
extern  double uart_gpst ;
__IO uint64_t cnt_on_pps;                         		/*CNT计数*/

