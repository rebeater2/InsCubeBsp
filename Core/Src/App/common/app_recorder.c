/**
* @file app_record.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/10 10:13
* @version 1.0
**/

#include <stdio.h>
#include "app_recorder.h"
#include "fatfs.h"
#include "string.h"
app_recorder_error_t app_recorder_init(app_recorder_t *recorder) {
	FRESULT res = f_mount(&SDFatFS, SDPath, 1);        /*挂载SD卡*/
	if (res != FR_OK) {
		recorder->trace("Error,SD card %s mount failed(%d)\n", recorder->filename, res);
		return app_recorder_mount_failed;
	}
	res = f_open(recorder->fp, recorder->filename, FA_WRITE | FA_CREATE_ALWAYS);
	if (res != FR_OK) {
		recorder->trace("Error,file %s create failed(%d)\n", recorder->filename, res);
		return app_recorder_create_failed;
	}
	recorder->byte_cnt = 0;
	return app_recorder_ok;

}

app_recorder_t *app_recorder_create(const char *filename) {
	app_recorder_t *p = bsp_malloc(sizeof(app_recorder_t));
	static FIL fil __attribute__((section (".ram_d1")));
	p->fp = &fil;
	p->buffer_size = 512;
	p->buffer = bsp_malloc(p->buffer_size);
	if (!p->buffer) {
		bsp_free(p->buffer);
		bsp_free(p);
		return NULL;
	}
/*	if(len > MAX_FILENAME_LEN){
		len = MAX_FILENAME_LEN;
	}*/
    if(!filename){
		char name[] = "LOG0.bin";
		strcpy(p->filename, name);
    }else{
        strcpy(p->filename, filename);
    }
	return p;
}

uint32_t str2hash(const char *str) {
	unsigned int hash = 0;
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int i;
	for (i = 0; str[i] != '\0'; i++) {
		hash = hash * a + str[i];
		a = a * b;
	}
	return hash;
}

typedef struct {
  uint32_t header;
  uint32_t ID;
  uint32_t length;
  system_time_t tm;
} app_recorder_header_t;
app_recorder_error_t app_recorder_push(app_recorder_t *recorder, uint8_t *buffer, size_t size) {
	memcpy(recorder->buffer, buffer, size);
	UINT write_length;
	FRESULT fr = f_write(recorder->fp, recorder->buffer, size, &write_length);
	if (fr != FR_OK && recorder->trace) {
		recorder->trace("Error,write failed(%d)\n", fr);
		return app_recorder_write_failed;
	}
	recorder->byte_cnt += size;
    /*每写入1MB sync一下*/
	if (recorder->byte_cnt >= 1024*1024) {
		fr = f_sync(recorder->fp);
		if (fr != FR_OK && recorder->trace) {
			recorder->trace("Error,f_sync failed(%d)\n", fr);
		}else{
            recorder->trace("f_sync once(%d)\n", fr);
        }
		recorder->byte_cnt = 0;
	}
	return app_recorder_ok;
}
