/**
* @file bdkyimu.c in FusionUnitXIH6
* @author rebeater
* @comment Driver for KU-IMU-102-A0, a mems IMU
* Create on 10/29/21 10:33 AM
* @version 1.0
**/
#include "bdkyimu.h"
#include<spi.h>
#include "main.h"
#include "cmsis_os.h"
#define HSPI hspi5
#define SPI_CS_DOWN()  HAL_GPIO_WritePin(SPI5_CS_GPIO_Port,SPI5_CS_Pin,GPIO_PIN_RESET)
#define SPI_CS_UP()  HAL_GPIO_WritePin(SPI5_CS_GPIO_Port,SPI5_CS_Pin,GPIO_PIN_SET)

#define TEMP_LOW	   0x0C
#define TEMP_OUT       0x0E

#define X_GYRO_LOW     0x10
#define X_GYRO_OUT     0x12
#define Y_GYRO_LOW     0x14
#define Y_GYRO_OUT     0x16
#define Z_GYRO_LOW     0x18
#define Z_GYRO_OUT     0x1A

#define X_ACCL_LOW     0x1C
#define X_ACCL_OUT     0x1E
#define Y_ACCL_LOW     0x20
#define Y_ACCL_OUT     0x22
#define Z_ACCL_LOW     0x24
#define Z_ACCL_OUT     0x26

//#define X_MAGN_OUT     0x2800		// x轴磁力计输出，高位字
//#define Y_MAGN_OUT     0x2A00       // y轴磁力计输出，高位字
//#define Z_MAGN_OUT     0x2C00       // z轴磁力计输出，高位字

//#define BAROM_LOW      0x2E00       // 气压计输出，低位字
//#define BAROM_OUT      0x3000       // 气压计输出，高位字

#define PROD_LOW	   0x7C
#define PROD_ID        0x7E

#define X_GYRO          1
#define Y_GYRO          2
#define Z_GYRO          3
#define X_ACCL          4
#define Y_ACCL          5
#define Z_ACCL          6
#define PAGE_ID			0x00

void KySpiWriteReg(uint8_t addr, uint8_t cmd) {
  uint8_t dat[4];
  SPI_CS_DOWN();
  dat[0] = addr | 0x80u;
  dat[1] = cmd;
  HAL_SPI_Transmit(&HSPI, (uint8_t *) dat, 1, 0xFFFF);
  SPI_CS_UP();
}

/*一个寄存器两个字节*/
uint16_t KySpiReadReg(uint8_t addr) {
//  uint8_t dat[4] = {0};
  uint8_t dat[4];
  dat[1] = addr;
  dat[0] = 0x00;
  SPI_CS_DOWN();
  HAL_SPI_TransmitReceive(&HSPI, dat, dat, 1, 0xfff);
  HAL_SPI_Receive(&HSPI, dat, 2, 0xfff);
  SPI_CS_UP();
  return *(uint16_t *)(&dat[0]);
}

void KyImu102A0ReadRaw(ImuRawKy *raw){
  uint16_t *data =(uint16_t *) raw;
  uint8_t addr = TEMP_LOW;
  for(int i = 0; i < 14; i++){
    data[i] = KySpiReadReg(addr + 2 * i);
  }
}

int KyImu102A0Initial() {
  KySpiWriteReg(0X80,00);
  delay(1000);
  uint16_t reg;
  do {
    reg = KySpiReadReg(PROD_ID);
    delay(100);
  } while (reg != 102);
	return 0;
};



