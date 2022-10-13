/**
* @file bsp_os.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/11 12:42
* @version 1.0
**/

#include "bsp.h"
#include "cmsis_os.h"

void bsp_create_task(bsp_task_t *p) {
	if (p->priority > osPriorityRealtime) {
		p->priority = osPriorityRealtime;
	}
	if (p->priority < osPriorityIdle) {
		p->priority = osPriorityIdle;
	}
	xTaskCreate(p->task_fn,
				p->task_name,
				p->stack_size,
				p->parameter,
				p->priority,
				(TaskHandle_t *)p->handle);

}

void bsp_suspend_task(bsp_task_t *p) {
	vTaskSuspend((TaskHandle_t )p->handle);
}
void bsp_delete_task(bsp_task_t *p){
	vTaskDelete(p->handle);
}

void bsp_cpu_timeinfo(char *buffer){
    vTaskGetRunTimeStats(buffer);
};