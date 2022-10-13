/**
* @file bsp.c in InsCube
* @author linfe
* @comment 驱动层代码，隔绝应用层和底层硬件，绝不允许在算法层直接调用底层HAL或者标准库函数，一律包含此文件
* Create on 2022/8/25 23:21
* @version 1.0
**/

#include "bsp.h"


void bsp_init(){
/*好像没什么需要做的=/=*/
}

/*public*/
bsp_io_error_t dev_write(bsp_device_t *dev, uint8_t *buffer, size_t size) {
	if(dev->write){
		size_t s = dev->write(buffer,size);
		if (s == size){
			return bsp_io_ok;
		}else{
			return bsp_io_wd_error;
		}
	}
	return bsp_io_not_supported;
}
size_t dev_read(bsp_device_t *dev, uint8_t *buffer, size_t size) {
	if(dev->read){
		size_t s = dev->read(buffer,size);
		if (s == size){
			return bsp_io_ok;
		}else{
			return bsp_io_wd_error;
		}
	}
	return bsp_io_not_supported;
}

bsp_io_error_t dev_ioctl(bsp_device_t *dev, bsp_io_state_t state){
	if(dev->ioctl){
		return	dev->ioctl(state);
	}
	return bsp_io_not_supported;
}



bsp_io_error_t dev_init(bsp_device_t *dev) {
	if(dev->init){
		return dev->init();
	}
	return bsp_io_not_supported;
}

bsp_io_error_t dev_deinit(bsp_device_t *dev) {
	if(dev->deinit){
		return 	dev->deinit();
	}
	return bsp_io_not_supported;
}

void bsp_delay_ms(uint32_t ms){
	osDelay(ms);
}