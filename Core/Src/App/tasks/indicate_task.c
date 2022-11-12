/**
* @file indecate_task.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/15 16:09
* @version 1.0
**/


#include "indicate_task.h"
#include "SensorFusion.h"
typedef struct {
  FusionStatus_t status;
  rgb_led_color_t color;
  uint32_t up_time_ms;
  uint32_t down_time_ms;
} fusion_status_indicate_t;

static fusion_status_indicate_t all_status_indicates[] = {
    {FusionStatusInit, BSP_LED_CYAN, 1000, 1000},
    {FusionStatusAlign, BSP_LED_CYAN, 200, 800},
    {FusionStatusINS, BSP_LED_PINK, 500, 1500},
    {FusionStatusFloat, BSP_LED_BLUE, 500, 1500},
    {FusionStatusFix, BSP_LED_GREEN, 500, 1500},
};
#define INDICATE_MAX_DELAY 0

_Noreturn void led_indicate_task(void *p) {
    led_task_para_t *task = (led_task_para_t *) p;
    rgb_led_blink(task->rgb_led);
    FusionStatus_t status;
    fusion_status_indicate_t indicate = all_status_indicates[0];
    for (;;) {
        if (bsp_msg_get(task->indicate_queue, (uint8_t *) &status, INDICATE_MAX_DELAY) == bsp_os_ok) {
            for (int i = 0; i < sizeof(all_status_indicates) / (sizeof(fusion_status_indicate_t)); ++i) {
                if (status == all_status_indicates[i].status) {
                    indicate = all_status_indicates[i];
                    break;
                }
            }
        }
        rgb_led_set_color(task->rgb_led, indicate.color);
        task->rgb_led->delay_ms(indicate.up_time_ms);
        if (indicate.down_time_ms > 0){
            rgb_led_set_color(task->rgb_led, BSP_LED_OFF);
            task->rgb_led->delay_ms(indicate.down_time_ms);
        }
    }
}