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
                (TaskHandle_t *) p->handle);
}

void bsp_suspend_task(bsp_task_t *p) {
    vTaskSuspend((TaskHandle_t) p->handle);
}
void bsp_delete_task(bsp_task_t *p) {
    vTaskDelete(p->handle);
}

void bsp_cpu_timeinfo(char *buffer) {
    vTaskGetRunTimeStats(buffer);
};

void bsp_msg_init(bsp_msg_t *msg) {
    msg->queue = xQueueCreate(msg->capacity, msg->item_size);
}
bsp_os_error_t bsp_msg_send(bsp_msg_t *msg, void *item) {
    if (msg->isr_send == 1) {
        BaseType_t woke;
        if (xQueueSendFromISR(msg->queue, item, &woke) == pdTRUE) {
            return bsp_os_ok;
        } else {
            return bsp_os_msg_full;
        }
    } else {
        if (xQueueSend(msg->queue, item, 1) == pdTRUE) {
            return bsp_os_ok;
        } else {
            return bsp_os_msg_full;
        }
    }
}
bsp_os_error_t bsp_msg_get(bsp_msg_t *p, void *buffer, uint32_t max_delay) {
    if (max_delay > portMAX_DELAY) {
        max_delay = portMAX_DELAY;
    }
    if (xQueueReceive(p->queue, buffer, max_delay) == pdTRUE) {
        return bsp_os_ok;
    } else {
        return bsp_os_msg_timeout;
    }
}