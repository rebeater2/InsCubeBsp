/**
* @file rgb_led.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/3 22:13
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_DRIVERS_RGB_LED_H_
#define INSCUBEBSP_CORE_SRC_DRIVERS_RGB_LED_H_
#include "../Bsp/bsp.h"

typedef struct {
  bsp_device_t *gpio_r;
  bsp_device_t *gpio_g;
  bsp_device_t *gpio_b;
  void (*delay_ms)(uint32_t ms);
}rgb_led_t;
typedef enum  {
  BSP_LED_OFF = 0,
  BSP_LED_RED = 1,
  BSP_LED_GREEN = 2,
  BSP_LED_YELLOW = 3,
  BSP_LED_BLUE = 4,
  BSP_LED_PINK = 5,
  BSP_LED_CYAN = 6,
  BSP_LED_WHITE = 7
}rgb_led_color_t;

void rgb_led_init(rgb_led_t *rgb_led);
void rgb_led_blink(rgb_led_t *dev);
void rgb_led_set_color(rgb_led_t *dev,rgb_led_color_t c);
rgb_led_t *led_handle_create();


#endif //INSCUBEBSP_CORE_SRC_DRIVERS_RGB_LED_H_
