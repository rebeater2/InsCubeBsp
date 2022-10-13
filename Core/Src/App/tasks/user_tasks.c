
#include "../../Drivers/rgb_led.h"
#include "../../Drivers/imu_adis1646x.h"
#include "../common/trace.h"
#include "recorder_task.h"

typedef struct {
  system_time_t tm;
  imu_raw_adi_t raw;
} imu_msg_t;

typedef struct {
  void (*error_handle)(const char *);
  adis1646x_handle_t *adi_handle;
  QueueHandle_t imu_msg_queue;
  QueueHandle_t imu_recorder_queue;
} imu_task_para_t;

/**
 * imu中断服务函数
 * @param tm 系统时间
 * @param p 参数指针
 */
void adi_data_ready_callback(system_time_t *tm, void *p) {

	BaseType_t woke;
	imu_msg_t msg;

	msg.tm = *tm;
	imu_task_para_t *task = (imu_task_para_t *)p;
	adis1646x_read(task->adi_handle, &msg.raw);
	recorder_data_t recorder_data={
		.type = RECORDER_TYPE_IMU,
		.imu = msg.raw,
		.tm	 = *tm
	};
	if(xQueueSendFromISR(task->imu_recorder_queue, &recorder_data, &woke)!=pdTRUE){
		common_trace("ISR queue is full\n");
	}
}

_Noreturn void error_handle(const char *s) {
	for (;;) {
		common_trace("Fetal error,task will be suspend due to %s\n", s);
		bsp_delay_ms(1000);
	}
}

typedef struct {
  void (*error_handle)(const char *);
  rgb_led_t *rgb_led;
} led_task_para_t;



_Noreturn void led_indicate_task(void *p) {
	led_task_para_t *task = (led_task_para_t *)p;
	rgb_led_blink(task->rgb_led);
	for (;;) {
		task->rgb_led->delay_ms(1000u);
		rgb_led_set_color(task->rgb_led, BSP_LED_BLUE);
		task->rgb_led->delay_ms(1000u);
		rgb_led_set_color(task->rgb_led, BSP_LED_OFF);
	}
}

void imu_init(imu_task_para_t *imu_task){
    adis1646x_handle_t *imu_dev = imu_task->adi_handle;
    imu_dev->trace = common_trace;
    adis1646x_error_t err = adis1646x_init(imu_dev);
    if (err == adis1646x_ok) {
        /*注册IMU的DR(data ready)中断服务函数*/
        register_gpio_irq(imu_dev->data_ready_pin, adi_data_ready_callback, (void *)imu_task);
    } else {
        error_handle("imu initial failed\n");
    }
}

_Noreturn void MainTask() {
	common_trace("main task\n");

	led_task_para_t led_para = {
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

	/*初始化日志任务*/
	QueueHandle_t recorder_queue = xQueueCreate(1000, sizeof(recorder_data_t));
	recorder_task_para_t recorder_task_para={
		.error_handler =error_handle,
		.recorder = app_recorder_create("LOG.dat"),
		.recorder_queue = recorder_queue,
	};
	recorder_task_para.recorder->trace = common_trace;
	bsp_task_t rec_task ={
		.task_name = "recorder task",
		.task_fn = recorder_task,
		.priority = osPriorityNormal,
		.stack_size = 1024,
		.parameter = &recorder_task_para,
	};
	bsp_create_task(&rec_task);

	/*初始化imu读取任务*/
    imu_task_para_t para = {
        .adi_handle = imu_handle_create(),
        .error_handle = error_handle,
        .imu_msg_queue = xQueueCreate(10, sizeof(imu_msg_t)),
        .imu_recorder_queue = recorder_queue,
    };
    imu_init(&para);



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
    char buffer[256];
	while (1) {
        bsp_cpu_timeinfo(buffer);
        common_trace("%s\n",buffer);
		osDelay(5000);
	}


}

