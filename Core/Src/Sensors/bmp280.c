/**
* @file bmp280.c in InsCube
* @author rebeater
* @comment
* Create on 3/6/22 6:19 PM
* @version 1.0
**/
#include "main.h"
//#include "co"
#include "bmp280.h"
typedef enum {
  BMP280_REGISTER_DIG_T1 = 0x88,
  BMP280_REGISTER_DIG_T2 = 0x8A,
  BMP280_REGISTER_DIG_T3 = 0x8C,

  BMP280_REGISTER_DIG_P1 = 0x8E,
  BMP280_REGISTER_DIG_P2 = 0x90,
  BMP280_REGISTER_DIG_P3 = 0x92,
  BMP280_REGISTER_DIG_P4 = 0x94,
  BMP280_REGISTER_DIG_P5 = 0x96,
  BMP280_REGISTER_DIG_P6 = 0x98,
  BMP280_REGISTER_DIG_P7 = 0x9A,
  BMP280_REGISTER_DIG_P8 = 0x9C,
  BMP280_REGISTER_DIG_P9 = 0x9E,

  BMP280_REGISTER_CHIPID = 0xD0,
  BMP280_REGISTER_VERSION = 0xD1,
  BMP280_REGISTER_SOFTRESET = 0xE0,

  BMP280_REGISTER_CAL26 = 0xE1,  // R calibration stored in 0xE1-0xF0

  BMP280_REGISTER_CONTROL = 0xF4,
  BMP280_REGISTER_CONFIG = 0xF5,
  BMP280_REGISTER_PRESSUREDATA = 0xF7,
  BMP280_REGISTER_TEMPDATA = 0xFA,
} BMP280_REG;
typedef struct {
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;

  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
  int16_t reserved;
} Coefficients;
/** Oversampling rate for the sensor. */
enum sensor_sampling {
  /** No over-sampling. */
  SAMPLING_NONE = 0x00,
  /** 1x over-sampling. */
  SAMPLING_X1 = 0x01,
  /** 2x over-sampling. */
  SAMPLING_X2 = 0x02,
  /** 4x over-sampling. */
  SAMPLING_X4 = 0x03,
  /** 8x over-sampling. */
  SAMPLING_X8 = 0x04,
  /** 16x over-sampling. */
  SAMPLING_X16 = 0x05
};
/** Operating mode for the sensor. */
enum sensor_mode {
  /** Sleep mode. */
  MODE_SLEEP = 0x00,
  /** Forced mode. */
  MODE_FORCED = 0x01,
  /** Normal mode. */
  MODE_NORMAL = 0x03,
  /** Software reset. */
  MODE_SOFT_RESET_CODE = 0xB6
};
/** Filtering level for sensor data. */
enum sensor_filter {
  /** No filtering. */
  FILTER_OFF = 0x00,
  /** 2x filtering. */
  FILTER_X2 = 0x01,
  /** 4x filtering. */
  FILTER_X4 = 0x02,
  /** 8x filtering. */
  FILTER_X8 = 0x03,
  /** 16x filtering. */
  FILTER_X16 = 0x04
};

/** Standby duration in ms */
enum standby_duration {
  /** 1 ms standby. */
  STANDBY_MS_1 = 0x00u,
  /** 62.5 ms standby. */
  STANDBY_MS_63 = 0x01u,
  /** 125 ms standby. */
  STANDBY_MS_125 = 0x02u,
  /** 250 ms standby. */
  STANDBY_MS_250 = 0x03u,
  /** 500 ms standby. */
  STANDBY_MS_500 = 0x04u,
  /** 1000 ms standby. */
  STANDBY_MS_1000 = 0x05u,
  /** 2000 ms standby. */
  STANDBY_MS_2000 = 0x06u,
  /** 4000 ms standby. */
  STANDBY_MS_4000 = 0x07u
};
/**/
int64_t t_fine = 0;
Coefficients coeff;
extern Coefficients coeff;

/*bsp for bmp280*/
inline void Bmp280SpiCsDown(Bmp280HandleDef *dev) {
  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}
inline void Bmp280SpiCsUp(Bmp280HandleDef *dev) {
  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

uint8_t Bmp280SpiReadReg(Bmp280HandleDef *dev, uint8_t reg) {
  uint8_t cmd = reg | 0x80u;
  Bmp280SpiCsDown(dev);
  HAL_SPI_Transmit(dev->hspi, &cmd, 1, 0xfff);
  HAL_SPI_Receive(dev->hspi, &cmd, 1, 0xfff);
  Bmp280SpiCsUp(dev);
  return cmd;
}
void Bmp280SpiWriteReg(Bmp280HandleDef *dev, uint8_t cmd, uint8_t val) {
  uint8_t dat[2];
  cmd &= ~0x80u;
  dat[0] = cmd;
  dat[1] = val;
  Bmp280SpiCsDown(dev);
  HAL_SPI_Transmit(dev->hspi, dat, 2, 0xffff);
  Bmp280SpiCsUp(dev);
}

uint16_t Bmp280SpiReadWord(Bmp280HandleDef *dev, uint8_t reg) {
  uint8_t buffer[2], cmd[2];
  cmd[0] = reg | 0x80u;
  uint16_t rslt;
  Bmp280SpiCsDown(dev);
  HAL_SPI_Transmit(dev->hspi, cmd, 1, 0xfff);
  HAL_SPI_Receive(dev->hspi, buffer, 2, 0xfff);
  Bmp280SpiCsUp(dev);
  rslt = ((uint16_t)buffer[0] << 8u) + (buffer[1]);
  return rslt;
}

uint16_t Bmp280ReadWordsLe(Bmp280HandleDef *dev, uint8_t reg) {
  uint16_t temp = Bmp280SpiReadWord(dev, reg);
  return (temp >> 8u) | (temp << 8u);
}

uint8_t Bmp280Init(Bmp280HandleDef *dev) {
  uint8_t reg;
  uint32_t failure_cnt = 0;
  do {
	reg = Bmp280SpiReadReg(dev, BMP280_REGISTER_CHIPID);
	if (dev->trace) {
	  dev->trace("Read bmp280 ID:%d\n", (int)reg);
	}
	failure_cnt++;
	dev->delay_ms(1000);
  } while (reg != 0x58 && failure_cnt < 5);
  if (failure_cnt >= 5) {
	dev->trace("Bmp280 ID read failed\n");
	return 1;
  }

  /*读取参数*/
  coeff.dig_P1 = (uint16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P1);
  coeff.dig_P2 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P2);
  coeff.dig_P3 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P3);
  coeff.dig_P4 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P4);
  coeff.dig_P5 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P5);
  coeff.dig_P6 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P6);
  coeff.dig_P7 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P7);
  coeff.dig_P8 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P8);
  coeff.dig_P9 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_P9);

  coeff.dig_T1 = (uint16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_T1);
  coeff.dig_T2 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_T2);
  coeff.dig_T3 = (int16_t)Bmp280ReadWordsLe(dev, BMP280_REGISTER_DIG_T3);

  uint8_t ctrl = (SAMPLING_X2 << 5u) | (SAMPLING_X16 << 2) | MODE_NORMAL;/**/
  Bmp280SpiWriteReg(dev, BMP280_REGISTER_CONTROL, ctrl);
  uint8_t cfg = STANDBY_MS_1 << 5u | FILTER_X16 << 2u | 0x00u;
  Bmp280SpiWriteReg(dev, BMP280_REGISTER_CONFIG, cfg);
  return 0;

}

float Bmp280ReadTemperature(Bmp280HandleDef *dev) {
  int32_t var1, var2;
  int32_t adc_t = Bmp280SpiReadWord(dev, BMP280_REGISTER_TEMPDATA);
  adc_t <<= 8u;
  adc_t |= Bmp280SpiReadReg(dev, BMP280_REGISTER_TEMPDATA + 2);
  adc_t >>= 4u;
  var1 = ((((adc_t >> 3u) - ((int32_t)coeff.dig_T1 << 1u))) *
	  ((int32_t)coeff.dig_T2)) >> 11u;
  var2 = (((((adc_t >> 4u) - ((int32_t)coeff.dig_T1)) *
	  ((adc_t >> 4u) - ((int32_t)coeff.dig_T1))) >> 12u) *
	  ((int32_t)coeff.dig_T3)) >> 14u;
  t_fine = var1 + var2;
  float T = (t_fine * 5 + 128) >> 8u;
  return T / 100.0;
}
float Bmp280ReadPressure(Bmp280HandleDef *dev) {
  int64_t var1, var2, p;
  int32_t adc_P = Bmp280SpiReadWord(dev, BMP280_REGISTER_PRESSUREDATA);
  adc_P <<= 8u;
  adc_P |= Bmp280SpiReadReg(dev, BMP280_REGISTER_PRESSUREDATA + 2);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)coeff.dig_P6;
  var2 = var2 + ((var1 * (int64_t)coeff.dig_P5) << 17);
  var2 = var2 + (((int64_t)coeff.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)coeff.dig_P3) >> 8) +
	  ((var1 * (int64_t)coeff.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)coeff.dig_P1) >> 33;

  if (var1 == 0) {
	return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)coeff.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)coeff.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)coeff.dig_P7) << 4);
  return (float)p / 256;
}
