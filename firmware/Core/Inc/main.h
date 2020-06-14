/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f3xx_hal.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_gpio.h"

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
#define M1_QDA_Pin GPIO_PIN_0
#define M1_QDA_GPIO_Port GPIOC
#define M1_QDB_Pin GPIO_PIN_1
#define M1_QDB_GPIO_Port GPIOC
#define M_SLEEP_Pin GPIO_PIN_2
#define M_SLEEP_GPIO_Port GPIOC
#define V12_SHUT_Pin GPIO_PIN_3
#define V12_SHUT_GPIO_Port GPIOC
#define M1_EN_Pin GPIO_PIN_0
#define M1_EN_GPIO_Port GPIOA
#define M2_EN_Pin GPIO_PIN_1
#define M2_EN_GPIO_Port GPIOA
#define SIG_DOCK_Pin GPIO_PIN_2
#define SIG_DOCK_GPIO_Port GPIOA
#define BEEP_PWM_Pin GPIO_PIN_3
#define BEEP_PWM_GPIO_Port GPIOA
#define AN_VBAT_Pin GPIO_PIN_4
#define AN_VBAT_GPIO_Port GPIOA
#define M2_QDB_Pin GPIO_PIN_7
#define M2_QDB_GPIO_Port GPIOA
#define AN_CURR_Pin GPIO_PIN_0
#define AN_CURR_GPIO_Port GPIOB
#define M3_EN_Pin GPIO_PIN_10
#define M3_EN_GPIO_Port GPIOB
#define M4_EN_Pin GPIO_PIN_11
#define M4_EN_GPIO_Port GPIOB
#define M4_QDA_Pin GPIO_PIN_6
#define M4_QDA_GPIO_Port GPIOC
#define M1_DIR_Pin GPIO_PIN_7
#define M1_DIR_GPIO_Port GPIOC
#define M2_DIR_Pin GPIO_PIN_8
#define M2_DIR_GPIO_Port GPIOC
#define M3_DIR_Pin GPIO_PIN_8
#define M3_DIR_GPIO_Port GPIOA
#define M4_DIR_Pin GPIO_PIN_9
#define M4_DIR_GPIO_Port GPIOA
#define LED_SYS_Pin GPIO_PIN_10
#define LED_SYS_GPIO_Port GPIOC
#define LED_DIAG_Pin GPIO_PIN_11
#define LED_DIAG_GPIO_Port GPIOC
#define M2_QDA_Pin GPIO_PIN_4
#define M2_QDA_GPIO_Port GPIOB
#define M3_QDA_Pin GPIO_PIN_6
#define M3_QDA_GPIO_Port GPIOB
#define M3_QDB_Pin GPIO_PIN_7
#define M3_QDB_GPIO_Port GPIOB
#define M4_QDB_Pin GPIO_PIN_8
#define M4_QDB_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
