/**
* @file bmp280.h in InsCube
* @author rebeater
* @comment Driver for BMP280 based on HAL/SPI
* Create on 3/6/22 6:56 PM
* @version 1.0
**/

#ifndef INSCUBE_CORE_INC_BMP280_H_
#define INSCUBE_CORE_INC_BMP280_H_
#include "main.h"

typedef struct{
  float pressure;
  float temperature;
}BmpRaw;
/**
 * BMP280 控制句柄
 */
typedef struct {
  SPI_HandleTypeDef *hspi;					/*SPI句柄*/
  GPIO_TypeDef *cs_port;					/*CS引脚*/
  uint16_t cs_pin;							/*CS引脚*/
  int (*delay_ms)(uint32_t ms);				/*延时函数,单位ms*/
  int (*trace)(const char *fmt, ...);		/*打印函数*/
} Bmp280HandleDef;
/**
 * BMP280初始化函数
 * @param dev
 * @return 0=OK  1:fail
 */
uint8_t Bmp280Init(Bmp280HandleDef *dev);

/**
 * Read current temperature
 * @param dev
 * @return 摄氏度
 * @note 温度应该在气压之前读取
 */
float Bmp280ReadTemperature(Bmp280HandleDef *dev);
/**
 * Read current pressure
 * @param dev
 * @return hPa 气压,单位百帕
 */
float Bmp280ReadPressure(Bmp280HandleDef *dev);
#endif //INSCUBE_CORE_INC_BMP280_H_
