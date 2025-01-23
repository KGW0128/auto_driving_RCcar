/*
 * RC_moter.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */

#include  "RC_moter.h"
#include "tim.h"

void Moter_Init()
{
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 모터 L용 PWM
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 모터 R용 PWM

  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
}


void Moter_Advance()//전진
{
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 0);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 0);

  TIM3->CCR1 = 100;
  TIM3->CCR2 = 100;


}
void Moter_Junior()//후진
{
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 0);

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 0);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);

  TIM3->CCR1 = 100;
  TIM3->CCR2 = 100;

}
void Moter_Right()//좌회전
{

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 0);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 0);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);

  TIM3->CCR1 = 100;
  TIM3->CCR2 = 100;

}
void Moter_Left()//우회전
{

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 0);

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 0);

  TIM3->CCR1 = 100;
  TIM3->CCR2 = 100;

}


void Moter_Stop()//정지
{

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
  HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);

  TIM3->CCR1 = 100;
  TIM3->CCR2 = 100;

}

