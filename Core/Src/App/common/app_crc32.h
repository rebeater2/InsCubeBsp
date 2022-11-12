/**
* @file app_crc32.h in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/24 23:27
* @version 1.0
**/

#ifndef INSCUBEBSP_CORE_SRC_APP_COMMON_APP_CRC32_H_
#define INSCUBEBSP_CORE_SRC_APP_COMMON_APP_CRC32_H_
#include "bsp.h"
uint32_t app_crc32_checksum(const uint8_t *indata, int num_bytes);
#endif //INSCUBEBSP_CORE_SRC_APP_COMMON_APP_CRC32_H_
