/**
* @file fusion_task.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/23 12:54
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_TASKS_FUSION_TASK_H_
#define INSCUBEBSP_CORE_SRC_APP_TASKS_FUSION_TASK_H_
#include "bsp.h"
#include "app_recorder.h"
#include "SensorFusion.h"
#define FUSION_PUSH_RESULT_FREQ 500
#define FUSION_PUSH_STATUS_FREQ 1



typedef struct {
  FusionOption_t *option;
  void (*error_handler)(const char *);
  int (*trace)(const char *fmt, ...);           /* 打印函数*/
  bsp_msg_t *recorder_que;
  bsp_msg_t *imu_msg_que;
  bsp_msg_t *gnss_msg_que;
  bsp_msg_t *indicate_que;

}fusion_task_para_t;

 void fusion_task(void *para);

#endif //INSCUBEBSP_CORE_SRC_APP_TASKS_FUSION_TASK_H_
