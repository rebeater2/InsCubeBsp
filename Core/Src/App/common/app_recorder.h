/**
* @file app_recorder.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/10 10:33
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_COMMON_APP_RECORDER_H_
#define INSCUBEBSP_CORE_SRC_APP_COMMON_APP_RECORDER_H_
#
#include "../../Bsp/bsp.h"
#define MAX_FILENAME_LEN 20
#define RECORD_TYPE(type) #type


typedef struct {
  uint32_t id;
  uint32_t length;
} app_recorder_item_t;



#define APP_TYPE_NAME(type)  #type
typedef enum {
  app_recorder_ok,
  app_recorder_mount_failed,
  app_recorder_create_failed,
  app_recorder_write_failed,
  app_recorder_flush_failed,
} app_recorder_error_t;

typedef struct {
  char filename[MAX_FILENAME_LEN];
  uint32_t byte_cnt;
  void *fp;
  uint8_t (*write)(uint8_t *buffer, size_t length);
  void (*delay_ms)(uint32_t ms);
  int (*trace)(const char *fmt, ...);
  uint8_t *buffer;
  uint32_t buffer_size;

} app_recorder_t;

app_recorder_t * app_recorder_create(const char *filename);
app_recorder_error_t app_recorder_init(app_recorder_t *recorder);

app_recorder_error_t app_recorder_push(app_recorder_t *recorder, uint8_t *buffer, size_t length);

#endif //INSCUBEBSP_CORE_SRC_APP_COMMON_APP_RECORDER_H_
