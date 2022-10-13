/**
* @file rtos_ext.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/14 0:04
* @version 1.0
**/

#include "rtos_ext.h"
#include "tim.h"
uint64_t rtos_run_count_value;
void rtos_htimer_config() {
    rtos_run_count_value = 0;
    HAL_TIM_Base_Start(&htim2);
}
void rots_htimer_callback() {
    rtos_run_count_value++;
}
inline uint64_t get_rtos_run_count_value() {
    return rtos_run_count_value;
}