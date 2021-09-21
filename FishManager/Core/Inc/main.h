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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define PWM_ADC0_Pin GPIO_PIN_0
#define PWM_ADC0_GPIO_Port GPIOA
#define PWM_ADC1_Pin GPIO_PIN_1
#define PWM_ADC1_GPIO_Port GPIOA
#define PWM_ADC2_Pin GPIO_PIN_2
#define PWM_ADC2_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_3
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_4
#define SW2_GPIO_Port GPIOA
#define PWM_IN2_Pin GPIO_PIN_6
#define PWM_IN2_GPIO_Port GPIOA
#define PWM_IN1_Pin GPIO_PIN_7
#define PWM_IN1_GPIO_Port GPIOA
#define PWM_IN3_Pin GPIO_PIN_0
#define PWM_IN3_GPIO_Port GPIOB
#define RSTB_Pin GPIO_PIN_1
#define RSTB_GPIO_Port GPIOB
#define A0_Pin GPIO_PIN_2
#define A0_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_10
#define SCL_GPIO_Port GPIOB
#define SI_Pin GPIO_PIN_11
#define SI_GPIO_Port GPIOB
#define DS18B20_Pin GPIO_PIN_12
#define DS18B20_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
extern void displayASC12X24_str(int x,int y,uint8_t p[]);
extern uint8_t DS18B20_Init(void);
extern short DS18B20_Get_Temp(void);
extern void init12864 (void);
extern void printf_lcd(uint8_t x,uint8_t y,char *fmt, ...);
extern void Soft_delay_us(uint16_t time);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
