/**
* @file recorder_task.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/15 23:23
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_TASKS_RECORDER_TASK_H_
#define INSCUBEBSP_CORE_SRC_APP_TASKS_RECORDER_TASK_H_
#include "../../App/common/app_recorder.h"
#include "../../Drivers/imu_adis1646x.h"


typedef struct {
  app_recorder_t *recorder;					/*define outside*/
  bsp_msg_t *recorder_queue;				/*message queue for different data*/
  void (*error_handler)(const char *msg); 	/*no return function when error occurred*/
  int (*trace)(const char *fmt, ...);
}recorder_task_para_t;

/**
 * task for recorder
 * @param p pointer to recorder_task_data_t
 */
_Noreturn void recorder_task(void *p);

#endif //INSCUBEBSP_CORE_SRC_APP_TASKS_RECORDER_TASK_H_
