/**
* @file bst_test.c in InsCube
* @author linfe
* @comment
* Create on 2022/8/25 23:40
* @version 1.0
**/

#include <stdio.h>
#include "bsp.h"


#include "../Drivers/imu_adis1646x.h"
#include "stm32h7xx_hal.h"

typedef struct{
  bsp_device_t dev;									/*设备描述子*/
  uint8_t buffer[1024];							/**/
  int (*delay_ms)(uint32_t ms);                /*延时函数,单位ms*/
  int (*trace)(const char *fmt, ...);            /*打印函数*/
}imu_device_t;

adis1646x_handle_t adis1646xHandle={

};


//void imu_init(imu_device_t *imu_dev){
//	adis1646xHandle.dev = spi5_dev;
//	adis1646xHandle.trace = printf;
//	adis1646xHandle.delay_ms = bsp_delay_ms;
//	adis1646x_init(&adis1646xHandle);
//
//
//
//
//	uint8_t buffer = 0xff;
//	dev_write(&imu_dev->dev,&buffer,1);
//	imu_dev->trace("hello world\n");
//}




void user_task_demo(){

	/* I/O device */
	dev_init(&spi1_dev);
	uint8_t buffer[1023];
	dev_write(&spi1_dev,buffer,1023);
	dev_deinit(&spi1_dev);

//	dev_init(&gpiod0_dev);
//	dev_ioctl(&gpiod0_dev,bsp_io_state_reset);
//	dev_write(&gpiod0_dev,buffer,1023);  /*not supported*/

	/* 行为设备控制 */
	imu_device_t imu_device ={
		.dev = spi1_dev,

	};
//	imu_init(&imu_device);
	uint16_t cs_pin;                            /*CS引脚*/
	int (*delay_ms)(uint32_t ms);                /*延时函数,单位ms*/
	int (*trace)(const char *fmt, ...);            /*打印函数*/

}