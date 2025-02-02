/*
 * sensor.h
 *
 *  Created on: Jan 31, 2025
 *      Author: USER
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "tim.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "delayus.h"


// 초음파 센서를 위한 구조체 정의
typedef struct {

	uint16_t IC_ValueRising;   // 상승 엣지에서 캡처된 타이머 값
	uint16_t IC_ValueFalling;  // 하강 엣지에서 캡처된 타이머 값
	uint16_t captureTime;      // 상승 ~ 하강 간의 시간 차이 (펄스 길이)
	uint8_t  captureFlag;      // 캡처 완료 플래그 (0: 캡처 시작, 1: 캡처 완료)
	uint8_t  distance;         // 계산된 거리 값 (단위: cm)
	GPIO_TypeDef* Port;        // 트리거 핀을 제어할 GPIO out포트
	uint16_t Pin;              // 트리거 핀 번호
	uint32_t tim_Channel;      // 연결된 타이머 채널

} HCSR04_Sensor;



// 초음파 센서 객체 선언
extern HCSR04_Sensor sensor1;
extern HCSR04_Sensor sensor2;
extern HCSR04_Sensor sensor3;


void HCSR04_Read(HCSR04_Sensor *sensor);// 초음파 센서에서 트리거 신호를 보내는 함수
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);// 타이머 입력 캡처 인터럽트 콜백 함수



#endif /* INC_SENSOR_H_ */
