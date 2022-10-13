/**
* @file app_common.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/9/4 9:24
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_COMMON_APP_COMMON_H_
#define INSCUBEBSP_CORE_SRC_APP_COMMON_APP_COMMON_H_

#include "../../Bsp/bsp.h"
typedef struct {
  char filename[20];
  size_t (*write)(uint8_t *buffer,size_t len);
  void (*flush)();
}app_recorder_t;

void app_record_init(app_recorder_t *recorder);
/**
 *
 * @param recorder
 * @param st
 * @param buffer
 * @param sz
 */
void app_record_push(app_recorder_t *recorder,system_time_t *st,void * buffer,size_t sz);

#endif //INSCUBEBSP_CORE_SRC_APP_COMMON_APP_COMMON_H_
