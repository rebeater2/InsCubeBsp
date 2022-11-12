/**
* @file trace.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 9:25
* @version 1.0
**/


#include "app_trace.h"
#include "bsp.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static SemaphoreHandle_t  mutex;

int common_trace(const char *fmt, ...) {
	char buffer[1024];
    xSemaphoreTake(mutex,0xf);
	va_list ap;
	int retval;
	va_start(ap, fmt);
	retval = vsprintf(buffer, fmt, ap);
	va_end(ap);
	dev_write(&uart1_dev, (uint8_t *)buffer,strlen(buffer));
    xSemaphoreGive(mutex);
	return retval;
}


void common_trace_init(){
	mutex = xSemaphoreCreateMutex();
}
