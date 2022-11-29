/**
* @file fusion_task.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/23 12:54
* @version 1.0
**/


#include <stdio.h>
#include <string.h>
#include "fusion_task.h"
#include "app_msgs.h"
#include "app_recorder.h"
#include "app_crc32.h"

#include "SensorFusion.h"

#define FUSION_TASK_MSG_DELAY 0xFFFFu

#pragma pack(8)
typedef struct {
  uint32_t header;
  uint32_t length;
  double tm;
  double lat;
  double lon;
  uint32_t id;
  float acce[3];
  float gyro[3];
  float height;
  float atti[3];
  uint32_t check_sum;
} output_data_t;
#pragma unpack()
static void fusion_push_out(FusionData_t *fd) {
    output_data_t out;
    out.header = 0xAA55AA55L;
    out.length = sizeof(out);
    out.tm = fd->cur_timestamp;
    for (int i = 0; i < 3; i++) {
        out.acce[i] = fd->imu.acce[i];
        out.gyro[i] = fd->imu.gyro[i];
        out.atti[i] = fd->atti[i];
    }
    out.lat = fd->latlng[0];
    out.lon = fd->latlng[1];
    out.height = fd->height;
    uint32_t check_sum = app_crc32_checksum((uint8_t *) &out, sizeof(out) - 4);
    uint8_t *p = (uint8_t *) &out.check_sum;
    for (int i = 0; i < 4; i++) {
        *p = (check_sum >> (8 * (3 - i))) & 0xff;
        p++;
    }
  /*  out.check_sum = check_sum;*/
/*    char buff[512];
    sprintf(buff,"%d %d %d %d\n",
            (int)out.tm,
            (int)(out.atti[0] * 180.0f/3.1415f),
            (int)(out.atti[1] * 180.0f/3.1415f),
            (int)(out.atti[2] * 180.0f/3.1415f)
            );*/
//    dev_write(&uart1_dev, (uint8_t *) &buff, strlen(buff));
    dev_write(&uart1_dev, (uint8_t *) &out, sizeof(out));

}
static void fusion_task_save_imu(fusion_task_para_t *task, imu_msg_t *imu_msg) {
    recorder_data_t recorder_data = {
        .type = RECORDER_TYPE_IMU,
        .imu = imu_msg->raw,
        .tm = imu_msg->tm
    };
    if (bsp_msg_send(task->recorder_que, &recorder_data) != bsp_os_ok) {
//        task->trace("%s: recorder is full\n", pcTaskGetTaskName(NULL));
    }
}
inline void msg_to_imu(imu_msg_t *msg, ImuData_t *imu_data, FusionOption_t *opt) {
    for (int i = 0; i < 3; ++i) {
        imu_data->acce[i] = opt->acce_scale * (float) msg->raw.acce[i];
        imu_data->gyro[i] = opt->gyro_scale * (float) msg->raw.gyro[i];
    }
}

_Noreturn void fusion_task(void *para) {
    const char *task_name = pcTaskGetTaskName(NULL);
    fusion_task_para_t *task = para;
    task->trace("%s: started\n", task_name);
    task->trace("%s: frame size %d\n", task_name, sizeof(output_data_t));
    task->trace("%s: sample rate %d\n", task_name, (int)(1.0f/task->option->delta_t));

    system_time_t tm_push_result = {0, 0};
    system_time_t tm_push_status = {0, 0};
    imu_msg_t imu_msg;
    ImuData_t imu_data;
    FusionStatus_t status;
    FusionData_t fd;
    fusion_init(&fd,task->option);
    while (1) {
        if (bsp_msg_get(task->imu_msg_que, &imu_msg, FUSION_TASK_MSG_DELAY) == bsp_os_ok) {
            fusion_task_save_imu(task, &imu_msg);
            /* fusion algorithm */
            msg_to_imu(&imu_msg, &imu_data, task->option);
            fusion_update(&fd,TM_TO_DOUBLE(imu_msg.tm), &imu_data, SensorIMU);
        }

        /*push to output device*/
        if (imu_msg.tm.millisecond > tm_push_result.millisecond) {
            fusion_push_out(&fd);
            tm_push_result.millisecond += 1000 / FUSION_PUSH_RESULT_FREQ;
        }

        /*push to indicate task*/
        if (imu_msg.tm.millisecond > tm_push_status.millisecond){
            fusion_getstatus(&fd,&status);
            bsp_msg_send(task->indicate_que,&status);
            tm_push_status.millisecond += 1000/FUSION_PUSH_STATUS_FREQ;
        }
    }
}