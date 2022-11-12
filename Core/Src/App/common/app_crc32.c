/**
* @file app_crc32.c in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/10/24 23:27
* @version 1.0
**/


#include "app_crc32.h"
#define CRC32_POLY 0x04C11DB7U
#define CRC32_START 0xFFFFFFFFU
#define HARDWARE_CRC 0
uint32_t app_crc32_checksum(const uint8_t *indata, int num_bytes) {
#if HARDWARE_CRC == 1
    #error "TODO: to be implement on arm"
    /*在STM32中通过硬件硬件实现*/
#else
    /*软件校验*/
    uint32_t crc = CRC32_START;
    const uint8_t *p = indata;
    for (int i = 0; i < num_bytes; i++) {
        crc = crc ^ (*p++ << 24U);
        for (int bit = 0; bit < 8; bit++) {
            if (crc & (1LU << 31U)) crc = (crc << 1U) ^ CRC32_POLY;
            else crc = (crc << 1U);
        }
    }
#endif
    return crc;
}