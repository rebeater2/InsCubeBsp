/**
* @file rtos_ext.h in InsCubeBsp
* @author linfe RTOS configure functions
* @comment
* Create on 2022/10/14 0:03
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_INC_RTOS_EXT_H_
#define INSCUBEBSP_CORE_INC_RTOS_EXT_H_
#include <stdint.h>
extern uint64_t rtos_run_count_value;
extern void rtos_htimer_config();
extern void rots_htimer_callback();
uint64_t get_rtos_run_count_value();
#endif //INSCUBEBSP_CORE_INC_RTOS_EXT_H_
