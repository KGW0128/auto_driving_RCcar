/*
 * wheel_motor.c
 *
 *  Created on: Jan 13, 2025
 *      Author: user
 */

#include "uart_bluetooth.h"
#include  "RC_moter.h"
#include <string.h>

//main에 있는 블루투스로 받았던 데이터 호출
extern uint8_t  rxData;

//클락션 및 후진음
extern uint8_t  buzzer_Check;

//led 깜박이 변수
extern uint8_t led_all;
extern uint8_t led_left;
extern uint8_t led_right;


//자율주행 모드 활성화 변수
extern uint8_t auto_drive_mode;


extern uint8_t speed;
extern uint8_t L_speed;
extern uint8_t R_speed;

void uart_bluetooth_call()
{

	switch(rxData)
	{

	case 'F'://전진
		if(auto_drive_mode==0)
		{
			Moter_Advance();
			HAL_UART_Transmit(&huart2, (uint8_t*)"Advance\r\n", strlen("Advance\r\n"), 0xFFFF);
		}
		break;

	case 'B'://후진
		if(auto_drive_mode==0)
		{
			Moter_Junior();
			HAL_UART_Transmit(&huart2, (uint8_t*)"Junior\r\n", strlen("Junior\r\n"), 0xFFFF);

			buzzer_Check = 1;
		}
		break;

	case 'L'://좌회전
		if(auto_drive_mode==0)
		{
			Moter_Left();
			HAL_UART_Transmit(&huart2, (uint8_t*)"Left\r\n", strlen("Left\r\n"), 0xFFFF);

			led_left=1;
		}
		break;

	case 'R'://우회전
		if(auto_drive_mode==0)
		{
			Moter_Right();
			HAL_UART_Transmit(&huart2, (uint8_t*)"Right\r\n", strlen("Right\r\n"), 0xFFFF);

			led_right=1;
		}
		break;

	case 'X'://엑스 정지
		if(auto_drive_mode==0)
		{
			Moter_Stop();
			HAL_UART_Transmit(&huart2, (uint8_t*)"Stop\r\n", strlen("Stop\r\n"), 0xFFFF);
		}
		break;


	case 'S'://네모 클락션
		buzzer_Check = 1;
		break;

	case 'T'://세모 쌍깜박이
		led_all ^= 1;
		break;

	case 'C'://동그라미 속도 변속

		if(speed == 80)
		{
			speed = 100;
			L_speed = 100;
			R_speed = 100;
		}
		else
		{
			speed = 80;
			L_speed = 80;
			R_speed = 80;
		}

		break;



	case 'A'://start 자율주행 모드
		auto_drive_mode ^= 1;
		break;


	default://버튼에서 손가락을 땠을 때, 또는 오류가 났을 때

		//자율주행 모드가 아니라면
		if(auto_drive_mode==0)
		{
			//모터 정지
			Moter_Stop();

			//좌,우회전 깜박이 끄기
			led_left=0;
			led_right=0;
		}

		//빈값 반환 확인
		HAL_UART_Transmit(&huart2, (uint8_t*)"data in 0\r\n", strlen("data in 0\r\n"), 0xFFFF);

		//부저 끄기
		buzzer_Check = 0;


		break;
	}


}


