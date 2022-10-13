/**
* @file drv_spi.c in InsCube
* @author linfe
* @comment
* Create on 2022/8/25 23:23
* @version 1.0
**/


#include "bsp.h"
#include "spi.h"

#define BSP_SPI_WRITE_DEF(num) \
size_t bsp_spi##num##_write(uint8_t *buffer, size_t length) {   \
HAL_GPIO_WritePin(SPI##num##_CS_GPIO_Port,SPI##num##_CS_Pin,GPIO_PIN_RESET); \
HAL_StatusTypeDef result = HAL_SPI_Transmit(&hspi##num, buffer, length, 0xfff);\
HAL_GPIO_WritePin(SPI##num##_CS_GPIO_Port,SPI##num##_CS_Pin,GPIO_PIN_SET);\
if (result == HAL_OK) {\
return length;\
} else {\
return 0;\
}\
}

#define BSP_SPI_READ_DEF(num) \
size_t bsp_spi##num##_read(uint8_t *buffer, size_t length) {   \
HAL_GPIO_WritePin(SPI##num##_CS_GPIO_Port,SPI##num##_CS_Pin,GPIO_PIN_RESET); \
HAL_StatusTypeDef result = HAL_SPI_Receive(&hspi##num, buffer, length, 0xfff);\
HAL_GPIO_WritePin(SPI##num##_CS_GPIO_Port,SPI##num##_CS_Pin,GPIO_PIN_SET);\
if (result == HAL_OK) {\
return length;\
} else {\
return 0;\
}\
}

#define BSP_SPI_INIT_DEF(num) 							\
bsp_io_error_t bsp_spi##num##_init (){     				\
    MX_SPI##num##_Init();								\
	return bsp_io_ok;									\
}
#define BSP_SPI_DEINIT_DEF(num) 						\
bsp_io_error_t bsp_spi##num##_deinit (){     				\
														\
	return bsp_io_ok;									\
}



#define BSP_SPI_DEVICE_DEF(num) \
BSP_SPI_INIT_DEF(num)			\
BSP_SPI_DEINIT_DEF(num)			\
BSP_SPI_WRITE_DEF(num)			\
BSP_SPI_READ_DEF(num)           \
bsp_device_t spi##num##_dev = {	\
	.write = bsp_spi##num##_write,		\
	.read = bsp_spi##num##_read,		\
	.init = bsp_spi##num##_init,		\
	.deinit=bsp_spi##num##_deinit		\
};




BSP_SPI_DEVICE_DEF(1)
BSP_SPI_DEVICE_DEF(4)
BSP_SPI_DEVICE_DEF(5)

