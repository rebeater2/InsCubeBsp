/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS ALIGN_RAM_D1;    /* File system object for SD logical drive */
FIL SDFile ALIGN_RAM_D1;       /* File object for SD */

/* USER CODE BEGIN Variables */

//#include "rtc"
#include "../../Core/Src/Sensors/nmea.h"
FIL IMUFile ALIGN_RAM_D1;
FIL GNSSRawFile ALIGN_RAM_D1;
FIL GNSSFile ALIGN_RAM_D1;
FIL RsltFile ALIGN_RAM_D1;
FIL VelFile ALIGN_RAM_D1;
FIL EntFile ALIGN_RAM_D1;
extern DateTime utc_time;
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  /* 不需要 RTC模块，GPS数据足够提供本地时间维持*/
  DWORD res = 0u;
  res |= ((uint32_t)utc_time.year << 25u);
  res |= ((uint32_t)(utc_time.mon) << 21u);
  res |= ((uint32_t)(utc_time.day) << 16u);
  res |= ((uint32_t)utc_time.hour << 11u);
  res |= ((uint32_t)utc_time.minute << 5u);
  res |= ((uint32_t)(utc_time.second / 2u) << 0u);
  return res;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
