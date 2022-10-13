/**
* @file drv_uart.c in InsCube
* @author linfe
* @comment
* Create on 2022/8/27 9:54
* @version 1.0
**/

#include "bsp.h"
#include "usart.h"

#define UART_WRITE_DEF(num) \
size_t uart_write_##num(uint8_t *buffer, size_t length){  \
    HAL_StatusTypeDef result = HAL_UART_Transmit(&huart##num,buffer,length,0xfff); \
    if(result == HAL_OK){ \
        return length;\
    }else{\
        return bsp_io_wd_error;\
    }\
}

#define UART_WRITE_DMA_DEF(num) \
size_t uart_write_##num(uint8_t *buffer, size_t length){  \
    HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(&huart##num,buffer,length); \
    if(result == HAL_OK){ \
        return length;\
    }else{\
        return bsp_io_wd_error;\
    }\
}

/*单纯不想复制代码*/
UART_WRITE_DEF(1)
UART_WRITE_DEF(2)
UART_WRITE_DEF(3)
UART_WRITE_DEF(4)
UART_WRITE_DEF(5)
UART_WRITE_DEF(6)



bsp_io_error_t uart1_init() {
	MX_USART1_UART_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart1_deinit() {
	return bsp_io_ok;
}
bsp_io_error_t uart2_init() {
	MX_USART2_UART_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart2_deinit() {
	return bsp_io_ok;
}
bsp_io_error_t uart3_init() {
	MX_USART3_UART_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart3_deinit() {
	return bsp_io_ok;
}
bsp_io_error_t uart4_init() {
	MX_UART4_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart4_deinit() {
	return bsp_io_ok;
}
bsp_io_error_t uart5_init() {
	MX_UART5_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart5_deinit() {
	return bsp_io_ok;
}
bsp_io_error_t uart6_init() {
	MX_USART6_UART_Init();
	return bsp_io_ok;
}
bsp_io_error_t uart6_deinit() {
	return bsp_io_ok;
}


/*
extern UART_HandleTypeDef huart1;
size_t uart1_write(uint8_t *buffer, size_t length){
	HAL_StatusTypeDef result = HAL_UART_Transmit(&huart1,buffer,length,0xfff);
	if(result == HAL_OK){
		return length;
	}else{
		return bsp_io_wd_error;
	}
}*/

bsp_device_t uart1_dev = {
	.write = uart_write_1,
	.read = NULL,
	.ioctl = NULL,
	.init = uart1_init,
	.deinit = uart1_deinit,
};

bsp_device_t uart2_dev = {
	.write = uart_write_2,
	.read = NULL,
	.ioctl = NULL,
	.init = uart2_init,
	.deinit = uart2_deinit,
};
bsp_device_t uart3_dev = {
	.write = uart_write_3,
	.read = NULL,
	.ioctl = NULL,
	.init = uart3_init,
	.deinit = uart3_deinit,
};

bsp_device_t uart4_dev = {
	.write = uart_write_4,
	.read = NULL,
	.ioctl = NULL,
	.init = uart4_init,
	.deinit = uart4_deinit,
};

bsp_device_t uart5_dev = {
	.write = uart_write_5,
	.read = NULL,
	.ioctl = NULL,
	.init = uart5_init,
	.deinit = uart5_deinit,
};

bsp_device_t uart6_dev = {
	.write = uart_write_6,
	.read = NULL,
	.ioctl = NULL,
	.init = uart6_init,
	.deinit = uart6_deinit,
};

