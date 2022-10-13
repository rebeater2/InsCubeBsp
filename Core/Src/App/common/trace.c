/**
* @file trace.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 9:25
* @version 1.0
**/


#include "trace.h"
#include "../../Bsp/bsp.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static QueueHandle_t mutex;

int common_trace(const char *fmt, ...) {
	char buffer[1024];
	va_list ap;
	int retval;
	va_start(ap, fmt);
	retval = vsprintf(buffer, fmt, ap);
	va_end(ap);

//	xQueueGetMutexHolder(mutex);
	dev_write(&uart1_dev, (uint8_t *)buffer,strlen(buffer));
//	xQueueGetMutexHolderFromISR(mutex);
	return retval;
}


void trace_init(){
	mutex = xQueueCreateMutex(queueQUEUE_TYPE_MUTEX );

}
