/**
* @file indecate_task.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/15 16:09
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_TASKS_INDECATE_TASK_H_
#define INSCUBEBSP_CORE_SRC_APP_TASKS_INDECATE_TASK_H_

#include "../../Bsp/bsp.h"
#include "../../Drivers/rgb_led.h"

typedef struct {
  void (*error_handle)(const char *);
  bsp_msg_t *indicate_queue;
  rgb_led_t *rgb_led;
} led_task_para_t;
_Noreturn void led_indicate_task(void *p);

#endif //INSCUBEBSP_CORE_SRC_APP_TASKS_INDECATE_TASK_H_
