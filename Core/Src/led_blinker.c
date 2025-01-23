/*
 * led_blinker.c
 *
 *  Created on: Jan 13, 2025
 *      Author: user
 */


#include "led_blinker.h"


//쌍깜박이
void LED_All_Timer_Interrupt(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

//왼쪽 깜박이
void LED_Left_Timer_Interrupt(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
}

//오른쪽 깜박이
void LED_Right_Timer_Interrupt(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}


//led 키기
void Start_Blinker(void)
{
  HAL_TIM_Base_Start_IT(&htim11);
}

//led 끄기
void Stop_Blinker(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
  HAL_TIM_Base_Stop_IT(&htim11);
}
