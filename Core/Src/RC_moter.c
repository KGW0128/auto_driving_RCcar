/*
 * RC_moter.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */

#include  "RC_moter.h"
#include "tim.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"  // FreeRTOS 헤더 추가

uint8_t left_count = 0, right_count = 0;  // 연속 감지 횟수(버벅임 방지용)


//전체 기본 스피드(가속,저속 받아오는 변수)
uint8_t speed = 80;

//좌우 개별 스피드
uint8_t L_speed = 80;
uint8_t R_speed = 80;



void Moter_Init()//초기 세팅
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 모터 L용 PWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 모터 R용 PWM

	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
}


void Moter_Advance()//전진
{
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 0);

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 0);

	TIM3->CCR1 = L_speed;
	TIM3->CCR2 = R_speed;


}
void Moter_Junior()//후진
{
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);

	TIM3->CCR1 = L_speed;
	TIM3->CCR2 = R_speed;

}
void Moter_Right()//우회전
{

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 0);

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);

	TIM3->CCR1 = L_speed;
	TIM3->CCR2 = R_speed;

}
void Moter_Left()//좌회전
{

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 0);

	TIM3->CCR1 = L_speed;
	TIM3->CCR2 = R_speed;

}


void Moter_Stop()//정지
{

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 1);


}



//자율주행 모드
void Moter_Auto_drive(uint8_t mid, uint8_t right, uint8_t left)
{


	//정면 장애물 감지 → 후진 후 회전
	if (mid <= 20)
	{
		Moter_Junior(); //후진

		//각 속도별 후진 지속시간 조절
		if(speed == 80)//저속
		{
			vTaskDelay(pdMS_TO_TICKS(300));  // 후진 지속 시간 (300ms)
		}
		else if(speed == 100)//가속
		{
			vTaskDelay(pdMS_TO_TICKS(100));  // 후진 지속 시간 (100ms)
		}


		//좌우 초음파 거리중 먼곳으로 회전
		if (right > left)
		{
			//회전용 속도조절
			L_speed = speed - 10;
			R_speed = speed - 20;

			Moter_Right(); // 우회전
		}
		else
		{
			//회전용 속도조절
			L_speed = speed - 20;
			R_speed = speed - 10;

			Moter_Left(); // 좌회전
		}

		//각 속도별 회전 지속시간 조절
		if(speed == 80)
		{
			vTaskDelay(pdMS_TO_TICKS(300));
		}
		else if(speed == 100)
		{
			vTaskDelay(pdMS_TO_TICKS(50));
		}


		//스피드 초기화
		L_speed = speed;
		R_speed = speed;

		 // 방향 감지 카운트 리셋
		left_count = 0;
		right_count = 0;
	}


	// 좌측 장애물 감지 → 일정 횟수 연속 감지 시 우회전
	else if (left <= 30)
	{
		//카운트 증가
		left_count++;

		if (left_count >= 3)  // 3회 이상 연속 감지 시 회전
		{
			L_speed = speed - 10;
			R_speed = speed - 20;
			Moter_Right(); // 우회전

			//vTaskDelay(pdMS_TO_TICKS(5));  // 회전 지속 시간

			//스피드 초기화
			L_speed = speed;
			R_speed = speed;

			//카운트 초기화
			left_count = 0;
		}
	}


	// 우측 장애물 감지 → 일정 횟수 연속 감지 시 좌회전
	else if (right <= 30)
	{
		//카운트 증가
		right_count++;

		if (right_count >= 3)  // 3회 이상 연속 감지 시 회전
		{
			L_speed = speed-20;
			R_speed = speed-10;
			Moter_Left(); // 좌회전

			//vTaskDelay(pdMS_TO_TICKS(5));  // 회전 지속 시간

			//스피드 초기화ㅏ
			L_speed = speed;
			R_speed = speed;

			//카운트 초기화
			right_count = 0;
		}
	}


	// 장애물이 없는 경우 → 전진
	else
	{
		Moter_Advance(); // 전진

		// 방향 감지 리셋
		left_count = 0;
		right_count = 0;
	}
}


