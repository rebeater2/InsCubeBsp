/**
* @file rgb_led.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/3 22:13
* @version 1.0
**/


#include "rgb_led.h"
void rgb_led_init(rgb_led_t *dev) {



}
void rgb_led_blink(rgb_led_t *dev) {
	for (int i = 0; i < 8; i++) {
		rgb_led_set_color(dev, (rgb_led_color_t)i);
		dev->delay_ms(300);
	}
	rgb_led_set_color(dev, BSP_LED_OFF);
}

void rgb_led_set_color(rgb_led_t *dev, rgb_led_color_t c) {
	switch (c) {
		case BSP_LED_RED: {
			dev_ioctl(dev->gpio_r, bsp_io_state_reset);
			dev_ioctl(dev->gpio_g, bsp_io_state_set);
			dev_ioctl(dev->gpio_b, bsp_io_state_set);
		}
			break;
		case BSP_LED_GREEN: {
			dev_ioctl(dev->gpio_r, bsp_io_state_set);
			dev_ioctl(dev->gpio_g, bsp_io_state_reset);
			dev_ioctl(dev->gpio_b, bsp_io_state_set);
		}
			break;
		case BSP_LED_BLUE: {
			dev_ioctl(dev->gpio_r, bsp_io_state_set);
			dev_ioctl(dev->gpio_g, bsp_io_state_set);
			dev_ioctl(dev->gpio_b, bsp_io_state_reset);
		}
			break;
		case BSP_LED_YELLOW: {
			dev_ioctl(dev->gpio_r, bsp_io_state_reset);
			dev_ioctl(dev->gpio_g, bsp_io_state_reset);
			dev_ioctl(dev->gpio_b, bsp_io_state_set);
		}
			break;
		case BSP_LED_PINK: {
			dev_ioctl(dev->gpio_r, bsp_io_state_reset);
			dev_ioctl(dev->gpio_g, bsp_io_state_set);
			dev_ioctl(dev->gpio_b, bsp_io_state_reset);
		}
			break;
		case BSP_LED_CYAN: {
			dev_ioctl(dev->gpio_r, bsp_io_state_set);
			dev_ioctl(dev->gpio_g, bsp_io_state_reset);
			dev_ioctl(dev->gpio_b, bsp_io_state_reset);
		}
			break;
		case BSP_LED_WHITE: {
			dev_ioctl(dev->gpio_r, bsp_io_state_reset);
			dev_ioctl(dev->gpio_g, bsp_io_state_reset);
			dev_ioctl(dev->gpio_b, bsp_io_state_reset);
		}
			break;
		case BSP_LED_OFF: {
			dev_ioctl(dev->gpio_r, bsp_io_state_set);
			dev_ioctl(dev->gpio_g, bsp_io_state_set);
			dev_ioctl(dev->gpio_b, bsp_io_state_set);
		}
			break;
		default:break;
	}
}

rgb_led_t *led_handle_create() {
	rgb_led_t *handle = bsp_malloc(sizeof(rgb_led_t));
	handle->gpio_r = &gpio_out_K4_dev;
	handle->gpio_g = &gpio_out_K5_dev;
	handle->gpio_b = &gpio_out_K6_dev;
	handle->delay_ms = bsp_delay_ms;
	return handle;
}