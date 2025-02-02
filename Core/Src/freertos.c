/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include <stdio.h>
#include "sensor.h"

#include  "RC_moter.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */




//#define SENSOR_SAMPLES 5  // 샘플 개수
//uint16_t sensor1_values[SENSOR_SAMPLES] = {0};
//
//
//
//uint16_t get_filtered_distance(HCSR04_Sensor *sensor)
//{
//	uint32_t sum = 0;
//
//	for (int i = 0; i < SENSOR_SAMPLES; i++)
//	{
//		HCSR04_Read(sensor);
//		sensor1_values[i] = sensor->distance;
//		osDelay(10); // 샘플 간 딜레이 추가
//	}
//
//	// 평균 계산
//	for (int i = 0; i < SENSOR_SAMPLES; i++)
//	{
//		sum += sensor1_values[i];
//	}
//	return sum / SENSOR_SAMPLES;
//}




//printf 사용하기 위한 코드
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	//uart2번 사용했기 때문에 설정
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}




// 초음파 센서 객체 선언 (초기값 0, 각각 GPIO 및 타이머 채널 할당)
HCSR04_Sensor sensor1 = {0, 0, 0, 0, 0, GPIOA, GPIO_PIN_12, TIM_CHANNEL_1};
HCSR04_Sensor sensor2 = {0, 0, 0, 0, 0, GPIOB, GPIO_PIN_5, TIM_CHANNEL_2};
HCSR04_Sensor sensor3 = {0, 0, 0, 0, 0, GPIOB, GPIO_PIN_4, TIM_CHANNEL_3};

extern uint8_t uart_call;


//main.c 부저 변수
extern uint8_t buzzer_Check;

//main.c led 깜박이 변수
extern uint8_t led_left;
extern uint8_t led_right;
extern uint8_t led_all;

uint8_t sensor_read_moter_stop = 0;
uint8_t moter_move_sensor_stop = 0;

uint8_t auto_drive_mode = 0;



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorTask */
osThreadId_t sensorTaskHandle;
const osThreadAttr_t sensorTask_attributes = {
  .name = "sensorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for moter_Task */
osThreadId_t moter_TaskHandle;
const osThreadAttr_t moter_Task_attributes = {
  .name = "moter_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for myMutex02 */
osMutexId_t myMutex02Handle;
const osMutexAttr_t myMutex02_attributes = {
  .name = "myMutex02"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

  /* creation of myMutex02 */
  myMutex02Handle = osMutexNew(&myMutex02_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of sensorTask */
  sensorTaskHandle = osThreadNew(StartTask02, NULL, &sensorTask_attributes);

  /* creation of moter_Task */
  moter_TaskHandle = osThreadNew(StartTask03, NULL, &moter_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for(;;)
	{
		//깜박이를 켰다면
		if(led_all==1||led_left==1||led_right==1)
		{
			Start_Blinker();
		}
		else
		{
			Stop_Blinker();
		}


		//클락션을 울렸다면
		if(buzzer_Check==1)
		{
			Start_Buzzer();
		}
		else
		{
			Stop_Buzzer();
		}



		osDelay(10);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the sensorTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	/* Infinite loop */
	for(;;)
	{

		//자율주행일 때 초음파 데이터 읽어오기
		if(auto_drive_mode == 1)
		{

			//			if(moter_move_sensor_stop==0)
			//			{
			//osMutexAcquire(myMutex01Handle, osWaitForever);

			sensor_read_moter_stop =1; // 모터를 잠시 정지
			osDelay(50);    // 노이즈 감소 대기




			//uint16_t filtered_distance = get_filtered_distance(&sensor1);
			//printf("sensor1: %d cm\n", filtered_distance);

			HCSR04_Read(&sensor1);
			printf("sensor1: %d cm\n",sensor1.distance);
			//osDelay(50);    // 노이즈 감소 대기


			//HCSR04_Read(&sensor2);
			//printf("sensor2: %d cm\n",sensor2.distance);


			//HCSR04_Read(&sensor3);
			//printf("sensor3: %d cm\n",sensor3.distance);

			sensor_read_moter_stop = 0;
			//osMutexRelease(myMutex01Handle);



			//}
		}


		osDelay(50);



	}
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
	/* Infinite loop */


	for(;;)
	{

		//		//조종
		//		if(uart_call == 1)
		//		{
		//			uart_bluetooth_call();
		//		}





		if(auto_drive_mode==1)
		{
			//uart_call = 0;


			HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN4_PIN, 0);
			HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_L_IN3_PIN, 1);

			HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN2_PIN, 1);
			HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTOR_R_IN1_PIN, 0);

			TIM3->CCR1 = 100;
			TIM3->CCR2 = 100;

		}
		//		else
		//		{
		//			uart_call = 1;
		//		}




		//		//자율주행
		//		if(sensor_read_moter_stop ==0)
		//		{
		//			moter_move_sensor_stop = 1;
		//			osDelay(10);
		//
		//
		//
		//
		//			osDelay(10);
		//			moter_move_sensor_stop = 0;
		//
		//		}


		osDelay(30);



	}
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

