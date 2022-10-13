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
typedef enum{
  RECORDER_TYPE_IMU = 0,
  RECORDER_TYPE_GNSS = 1,
}recorder_data_type_t;

typedef struct {
  recorder_data_type_t type;
  system_time_t tm;
  union {
	imu_raw_adi_t imu;
	/*other sensor data*/
  };
}recorder_data_t;

typedef struct {
  app_recorder_t *recorder;					/*define outside*/
  QueueHandle_t recorder_queue;				/*message queue for different data*/
  void (*error_handler)(const char *msg); 	/*no return function when error occurred*/
}recorder_task_para_t;

/**
 * task for recorder
 * @param p pointer to recorder_task_data_t
 */
_Noreturn void recorder_task(void *p);

#endif //INSCUBEBSP_CORE_SRC_APP_TASKS_RECORDER_TASK_H_
