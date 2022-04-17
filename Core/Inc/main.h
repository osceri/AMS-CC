/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define enable_AIR_plus_Pin GPIO_PIN_2
#define enable_AIR_plus_GPIO_Port GPIOE
#define enable_AIR_minus_Pin GPIO_PIN_3
#define enable_AIR_minus_GPIO_Port GPIOE
#define AIR_plus_closed_Pin GPIO_PIN_4
#define AIR_plus_closed_GPIO_Port GPIOE
#define AIR_minus_closed_Pin GPIO_PIN_5
#define AIR_minus_closed_GPIO_Port GPIOE
#define precharge_closed_Pin GPIO_PIN_6
#define precharge_closed_GPIO_Port GPIOE
#define precharge_resistor_temperature_Pin GPIO_PIN_1
#define precharge_resistor_temperature_GPIO_Port GPIOA
#define fuse_resistor_temperature_Pin GPIO_PIN_2
#define fuse_resistor_temperature_GPIO_Port GPIOA
#define aux_temp1_Pin GPIO_PIN_3
#define aux_temp1_GPIO_Port GPIOA
#define aux_temp2_Pin GPIO_PIN_4
#define aux_temp2_GPIO_Port GPIOA
#define WARNING_LED_Pin GPIO_PIN_5
#define WARNING_LED_GPIO_Port GPIOA
#define HEARTBEAT_LED_Pin GPIO_PIN_6
#define HEARTBEAT_LED_GPIO_Port GPIOA
#define OK_LED_Pin GPIO_PIN_7
#define OK_LED_GPIO_Port GPIOA
#define LTC_CS_Pin GPIO_PIN_4
#define LTC_CS_GPIO_Port GPIOC
#define error_LED_Pin GPIO_PIN_5
#define error_LED_GPIO_Port GPIOC
#define SC_probe_Pin GPIO_PIN_1
#define SC_probe_GPIO_Port GPIOB
#define IMD_ok_Pin GPIO_PIN_2
#define IMD_ok_GPIO_Port GPIOB
#define fan_PWM_Pin GPIO_PIN_13
#define fan_PWM_GPIO_Port GPIOE
#define AMS_error_latched_Pin GPIO_PIN_6
#define AMS_error_latched_GPIO_Port GPIOC
#define AMS_error_Pin GPIO_PIN_7
#define AMS_error_GPIO_Port GPIOC
#define IMD_error_latched_Pin GPIO_PIN_8
#define IMD_error_latched_GPIO_Port GPIOC
#define IMD_error_Pin GPIO_PIN_9
#define IMD_error_GPIO_Port GPIOC
#define IMD_data_Pin GPIO_PIN_15
#define IMD_data_GPIO_Port GPIOA
#define enable_precharge_Pin GPIO_PIN_1
#define enable_precharge_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define TICK2HZ (configTICK_RATE_HZ / 1.2) // Specific for using a STM32F446RE with STm32F407VG code ...

#define SIMULATION

#define STREAM_DATA

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
