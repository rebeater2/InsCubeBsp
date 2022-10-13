/**
* @file drv_gpio.c in InsCube
* @author linfe
* @comment
* Create on 2022/8/26 23:32
* @version 1.0
**/


#include "bsp.h"
#include "gpio.h"


#define GPIO_WRITE_DEF(port,pin) \
bsp_io_error_t gpio_write_##port##pin(bsp_io_state_t bsp_io_state){ 	\
	HAL_GPIO_WritePin(GPIO##port,GPIO_PIN_##pin,(GPIO_PinState)(bsp_io_state)); 	\
	return bsp_io_ok; 		\
}

#define GPIO_INIT_DEF(port,pin)  \
bsp_io_error_t gpio_##port##pin##_init(){\
	return bsp_io_ok;\
}
#define GPIO_DEINIT_DEF(port,pin) \
bsp_io_error_t gpio_##port##pin##_deinit(){\
	return bsp_io_ok;\
}




#define GPIO_OUTPUT_DEF(port,pin) \
GPIO_WRITE_DEF(port,pin)              \
GPIO_INIT_DEF(port,pin)              \
GPIO_DEINIT_DEF(port,pin)              \
                                  \
bsp_device_t gpio_out_##port##pin##_dev={  \
	.write = NULL,\
	.read = NULL,\
	.ioctl =gpio_write_##port##pin,\
	.init = gpio_##port##pin##_init,\
	.deinit = gpio_##port##pin##_deinit,\
};


/* PC13 --> LED*/

/**
 * #define RGB_LED_R_Pin GPIO_PIN_4
#define RGB_LED_R_GPIO_Port GPIOK
 #define RGB_LED_B_Pin GPIO_PIN_6
#define RGB_LED_B_GPIO_Port GPIOK
 #define RGB_LED_G_Pin GPIO_PIN_5
#define RGB_LED_G_GPIO_Port GPIOK
 * @param bsp_io_state
 * @return
 */
GPIO_OUTPUT_DEF(K,4)
GPIO_OUTPUT_DEF(K,5)
GPIO_OUTPUT_DEF(K,6)