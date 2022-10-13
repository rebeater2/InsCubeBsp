/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define UART_RX_BUFFER_LENGTH 2048
#define USE_FREERTOS 1
#if USE_FREERTOS == 1
#define delay(ms) osDelay(ms)
#else
#define delay(ms) HAL_Delay(ms)
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RGB_LED_G_Pin GPIO_PIN_5
#define RGB_LED_G_GPIO_Port GPIOK
#define SPI1_CS_0_Pin GPIO_PIN_10
#define SPI1_CS_0_GPIO_Port GPIOG
#define ICM_INT_0_Pin GPIO_PIN_4
#define ICM_INT_0_GPIO_Port GPIOD
#define ICM_INT_0_EXTI_IRQn EXTI4_IRQn
#define RGB_LED_R_Pin GPIO_PIN_4
#define RGB_LED_R_GPIO_Port GPIOK
#define GNSS_PPS_Pin GPIO_PIN_3
#define GNSS_PPS_GPIO_Port GPIOD
#define GNSS_PPS_EXTI_IRQn EXTI3_IRQn
#define RGB_LED_B_Pin GPIO_PIN_6
#define RGB_LED_B_GPIO_Port GPIOK
#define SPI4_CS_Pin GPIO_PIN_4
#define SPI4_CS_GPIO_Port GPIOE
#define GNSS_PPS_EXT_Pin GPIO_PIN_8
#define GNSS_PPS_EXT_GPIO_Port GPIOA
#define GNSS_PPS_EXT_EXTI_IRQn EXTI9_5_IRQn
#define SPI1_CS_3_Pin GPIO_PIN_7
#define SPI1_CS_3_GPIO_Port GPIOG
#define SPI1_CS_2_Pin GPIO_PIN_6
#define SPI1_CS_2_GPIO_Port GPIOG
#define ICM_INT_1_Pin GPIO_PIN_12
#define ICM_INT_1_GPIO_Port GPIOI
#define ICM_INT_1_EXTI_IRQn EXTI15_10_IRQn
#define ICM_INT_2_Pin GPIO_PIN_13
#define ICM_INT_2_GPIO_Port GPIOI
#define ICM_INT_2_EXTI_IRQn EXTI15_10_IRQn
#define ICM_INT_3_Pin GPIO_PIN_14
#define ICM_INT_3_GPIO_Port GPIOI
#define ICM_INT_3_EXTI_IRQn EXTI15_10_IRQn
#define SPI1_CS_1_Pin GPIO_PIN_3
#define SPI1_CS_1_GPIO_Port GPIOG
#define SPI5_CS_Pin GPIO_PIN_1
#define SPI5_CS_GPIO_Port GPIOK
#define ADI_INT_Pin GPIO_PIN_0
#define ADI_INT_GPIO_Port GPIOJ
#define ADI_INT_EXTI_IRQn EXTI0_IRQn
#define CAM_INT_Pin GPIO_PIN_15
#define CAM_INT_GPIO_Port GPIOE
#define CAM_INT_EXTI_IRQn EXTI15_10_IRQn
#define ETH_RESET_Pin GPIO_PIN_5
#define ETH_RESET_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#ifndef SPI1_CS_GPIO_Port
#define SPI1_CS_GPIO_Port SPI1_CS_0_GPIO_Port
#define SPI1_CS_Pin SPI1_CS_0_Pin
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
