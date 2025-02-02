/*
 * delayus.c
 *
 *  Created on: Jan 26, 2025
 *      Author: USER
 */


#include "delayus.h"


void delay_us(uint16_t us)
{

  //11번 타이머에 인스턴스에 count에다가 0을 집어넣음
  //중첩 구조체형식
  __HAL_TIM_SET_COUNTER(&htim10, 0);

  //타이머에 인스턴스 카운더를 가져옴
  //들어온 us시간이 넘을 때까지 대기
  while((__HAL_TIM_GET_COUNTER(&htim10)) < us);
}
