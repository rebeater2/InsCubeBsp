
#include <memory.h>
#include "rgb_led.h"
#include "app_msgs.h"
#include "app_trace.h"
#include "recorder_task.h"
#include "indicate_task.h"
#include "fusion_task.h"



typedef struct {
  void (*error_handle)(const char *);
  adis1646x_handle_t *adi_handle;
  bsp_msg_t *imu_msg_queue;
  bsp_msg_t *imu_recorder_queue;
} imu_task_para_t;


/**
 * imu中断服务函数
 * @param tm 系统时间
 * @param p 参数指针
 */
void adi_data_ready_callback(system_time_t *tm, void *p) {
    imu_msg_t msg;
    msg.tm = *tm;
    imu_task_para_t *task = (imu_task_para_t *) p;
    adis1646x_read(task->adi_handle, &msg.raw);

    if (bsp_msg_send(task->imu_msg_queue, &msg) != bsp_os_ok) {
//        common_trace("ISR queue is full\n");
    }
}

_Noreturn void error_handle(const char *s) {
    for (;;) {
        common_trace("Fetal error,task will be suspend due to %s\n", s);
        bsp_delay_ms(1000);
    }
}

void imu_irq_init(imu_task_para_t *imu_task) {
    adis1646x_handle_t *imu_dev = imu_task->adi_handle;
    imu_dev->trace = common_trace;
    adis1646x_error_t err = adis1646x_init(imu_dev);
    if (err == adis1646x_ok) {
        /*注册IMU的DR(data ready)中断服务函数*/
        register_gpio_irq(imu_dev->data_ready_pin, adi_data_ready_callback, (void *) imu_task);
    } else {
        error_handle("imu initial failed\n");
    }
}

_Noreturn void MainTask() {
    char *current_task_name = pcTaskGetName(NULL);
    common_trace_init();
    bsp_msg_t led_indicate_queue = {
        .queue = NULL,
        .isr_send = 0,
        .capacity = 1,
        .item_size = 1,
        .isr_get = 0
    };
    bsp_msg_init(&led_indicate_queue);
    led_task_para_t led_para = {
        .indicate_queue = &led_indicate_queue,
        .error_handle = error_handle,
        .rgb_led =  led_handle_create(),
    };
    bsp_task_t led_task = {
        .task_name = "led_task",
        .task_fn = led_indicate_task,
        .stack_size = 128,
        .priority=osPriorityIdle,
        .parameter = &led_para,
    };
    bsp_create_task(&led_task);

    /*初始化消息队列*/
    bsp_msg_t recorder_queue = {
        .queue = NULL,
        .item_size = sizeof(recorder_data_t),
        .capacity = 100,
        .isr_send = 0,
    };
    bsp_msg_init(&recorder_queue);
    bsp_msg_t imu_queue = {
        .queue = NULL,
        .item_size = sizeof(imu_msg_t),
        .capacity = 100,
        .isr_send = 1,
    };
    bsp_msg_init(&imu_queue);

    /*初始化日志任务*/
    recorder_task_para_t recorder_task_para = {
        .error_handler =error_handle,

        .recorder = NULL,
        .recorder_queue = &recorder_queue,
        .trace = common_trace,
    };
    bsp_task_t rec_task = {
        .task_name = "recorder task",
        .task_fn = recorder_task,
        .priority = osPriorityLow,
        .stack_size = 1024,
        .parameter = &recorder_task_para,
    };
    bsp_create_task(&rec_task);

    /** 初始化IMU中断*/
    imu_task_para_t imu_task_para = {
        .adi_handle = imu_handle_create(),
        .error_handle = error_handle,
        .imu_msg_queue = &imu_queue,
        .imu_recorder_queue = NULL,
    };
    imu_irq_init(&imu_task_para);

    /** 初始化组合导航任务 */
    FusionOption_t *opt = bsp_malloc(sizeof(FusionOption_t));
    opt->delta_t = imu_task_para.adi_handle->deltat;
        opt->acce_scale = imu_task_para.adi_handle->attrib->acce_scale;
        opt->gyro_scale = imu_task_para.adi_handle->attrib->gyro_scale;
    fusion_task_para_t fusion_para = {
        .trace = common_trace,
        .error_handler = error_handle,
        .recorder_que = &recorder_queue,
        .imu_msg_que = &imu_queue,
        .indicate_que = &led_indicate_queue,
        .option = opt,
    };
    bsp_task_t fusion_handle={
        .task_name = "fusion task",
        .parameter = &fusion_para,
        .stack_size = 2048*3,
        .task_fn = fusion_task,
        .priority = osPriorityHigh,
    };
    common_trace("create fusion task\n");
    bsp_create_task(&fusion_handle);

#if 0
    imu_task_para_t para = {
        .adi_handle = imu_handle_create(),
        .error_handle = error_handle,
        .sem_imu_ready =  xQueueCreate(256, sizeof(imu_msg_t)),
        .imu_msg_queue = xQueueCreate(10, sizeof(imu_msg_t)),
        .imu_recorder_queue = recorder_queue,
    };
    para.adi_handle->trace = common_trace;
    bsp_task_t imu_task = {
        .task_name="imu task",
        .task_fn = imu_read_task,
        .priority = osPriorityHigh,
        .stack_size = 1024,
        .parameter = &para,
    };
    bsp_create_task(&imu_task);
#endif
    char buffer[1000];
    memset(buffer, 0, 1000);
    while (1) {
//        bsp_cpu_timeinfo(buffer);
//        common_trace("system tick\n");
        osDelay(3000);
    }

}

