/*
 * sensor.c
 *
 *  Created on: Jan 31, 2025
 *      Author: USER
 */

#include "sensor.h"

#include "cmsis_os2.h"  // FreeRTOS CMSIS API
extern osMutexId_t myMutex02Handle;  // 뮤텍스 핸들 선언

// 초음파 센서에서 트리거 신호를 보내는 함수
void HCSR04_Read(HCSR04_Sensor *sensor)
{




	// 트리거 핀을 LOW로 설정하여 초기화
	HAL_GPIO_WritePin(sensor->Port, sensor->Pin, 0);
	osDelay(1);

	// 트리거 핀을 HIGH로 설정하여 초음파 펄스 송신
	HAL_GPIO_WritePin(sensor->Port, sensor->Pin, 1);
	osDelay(10);

	// 다시 LOW로 설정하여 트리거 신호 종료
	HAL_GPIO_WritePin(sensor->Port, sensor->Pin, 0);

	// 타이머 인터럽트 활성화 (에코 신호 수신 대기)
	__HAL_TIM_ENABLE_IT(&htim2, sensor->tim_Channel);


}



// 타이머 입력 캡처 인터럽트 콜백 함수
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	osMutexAcquire(myMutex02Handle, osWaitForever);

	HCSR04_Sensor* sensor = NULL;

	// 인터럽트 발생한 채널에 따라 적절한 센서 선택
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		sensor = &sensor1;
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		sensor = &sensor2;
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		sensor = &sensor3;
	}


	// 인터럽트가 발생하여 NULL값이 아니라면
	if (sensor)
	{
		if (sensor->captureFlag == 0) // 상승 엣지 감지 (에코 신호 시작)
		{
			sensor->IC_ValueRising = HAL_TIM_ReadCapturedValue(htim, sensor->tim_Channel);
			sensor->captureFlag = 1; // 캡처 상태 변경

			// 다음 인터럽트를 하강 엣지에서 발생하도록 설정
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, sensor->tim_Channel, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else // 하강 엣지 감지 (에코 신호 종료)
		{
			sensor->IC_ValueFalling = HAL_TIM_ReadCapturedValue(htim, sensor->tim_Channel);
			sensor->captureFlag = 0; // 캡처 완료

			// 타이머 카운터 초기화
			__HAL_TIM_SET_COUNTER(htim, 0);

			// 타이머 값 차이를 이용해 에코 신호 지속 시간을 계산
			if (sensor->IC_ValueFalling > sensor->IC_ValueRising)
				sensor->captureTime = sensor->IC_ValueFalling - sensor->IC_ValueRising;
			else
				sensor->captureTime = (0xffff - sensor->IC_ValueRising) + sensor->IC_ValueFalling;

			// 거리 계산 (음속을 고려하여 58로 나눔)
			sensor->distance = sensor->captureTime / 58;

			// 다시 상승 엣지를 감지하도록 설정
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, sensor->tim_Channel, TIM_INPUTCHANNELPOLARITY_RISING);

			// 타이머 인터럽트 재활성화 (다음 측정 준비)
			__HAL_TIM_ENABLE_IT(&htim2, sensor->tim_Channel);
		}
	}


	osMutexRelease(myMutex02Handle);

}

