/**
* @file recorder_task.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/15 23:23
* @version 1.0
**/


#include "recorder_task.h"
#include <stdio.h>
#include <math.h>
_Noreturn void recorder_task(void *p) {
    recorder_task_para_t *task = (recorder_task_para_t *) p;
    char *task_name =  pcTaskGetName(NULL);
    task->trace("%s: task start\n",   task_name);
    if (!task->error_handler) {
        for (;;) {
            bsp_delay_ms(0xFFFFFF);
        }
    }
    if (!task->recorder) {
       task->recorder = app_recorder_create(NULL);
       task->recorder->trace = task->trace; /*DO NOT be NULL!*/
    }
    app_recorder_init(task->recorder);
    if (!task->recorder_queue) {
        task->error_handler("recorder_queue is NULL\n");
    }
    task->trace("%s: recorder_data_t = %d\n",task_name ,sizeof(recorder_data_t));

    app_recorder_error_t recorder_error = app_recorder_init(task->recorder);
    if (recorder_error != app_recorder_ok) {
        task->trace("%s: recorder init failed(%d)\n",task_name, recorder_error);
        task->error_handler("recorder init failed\n");
    }
    task->trace("%s: log to %s\n",task_name, task->recorder->filename);
    recorder_data_t data;
    uint32_t data_cnt = 0;
    char out_buff[1024] = {0};
    system_time_t tm_pre = {0};
    while (1) {
        if (bsp_msg_get(task->recorder_queue, (uint8_t *) &data, 0x01) == pdTRUE) {
            if (app_recorder_push(task->recorder, (uint8_t *) &data, sizeof(data)) != app_recorder_ok) {
                task->error_handler("app recorder push failed\n");
            }
            system_time_t tm = data.tm;
            float delta_t = (float) ((int32_t)tm.millisecond - (int32_t)tm_pre.millisecond) * 1e-3f
                + (float) ((int32_t)tm.microsecond -(int32_t) tm_pre.microsecond) * 1e-6f;
            if (fabsf(delta_t) > 2 / 1024.0) {
//                task->trace("delta_t is too large %d *1e-6\n", (int) (1e6 * delta_t));
            }
            tm_pre = tm;
/*            if (data_cnt % 1000 == 0) {
                struct {
                  double gyro[3];
                  double acce[3];
                } imu;
                imu_raw_adi_t *imu_raw = &data.imu;
                imu.gyro[1] = adis1646x_kg * imu_raw->gyro[0];
                imu.gyro[0] = adis1646x_kg * imu_raw->gyro[1];
                imu.gyro[2] = -adis1646x_kg * imu_raw->gyro[2];
                imu.acce[1] = adis1646x_ka_g * imu_raw->acce[0];
                imu.acce[0] = adis1646x_ka_g * imu_raw->acce[1];
                imu.acce[2] = -adis1646x_ka_g * imu_raw->acce[2];
                int n = sprintf((char *) out_buff, " %.6f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
                                (double) tm.millisecond / 1000 + tm.microsecond / 1e6,
                                imu.gyro[0],
                                imu.gyro[1],
                                imu.gyro[2],
                                imu.acce[0],
                                imu.acce[1],
                                imu.acce[2]
                );
                dev_write(&uart1_dev, (uint8_t *) out_buff, n);
            }*/
            data_cnt++;
        }
    }
}