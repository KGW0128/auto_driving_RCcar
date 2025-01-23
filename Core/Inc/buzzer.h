/*
 * buzzer.h
 *
 *  Created on: Jan 13, 2025
 *      Author: user
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_


#include <stdint.h>  // uint16_t, uint8_t 사용을 위한 추가
#include "main.h"    // STM32 관련 헤더
#include "gpio.h"    // GPIO 관련 헤더
#include "tim.h"     // 타이머 관련 헤더

//음표
#define Do  383 //도
#define Re  340 //레
#define Mi  304 //미
#define Fa  287 //파
#define Sol 255 //솔
#define La  227 //라
#define Si  203 //시
#define DDo 191 //높은 도







void Buzzer_Timer_Interrupt(void);
void Start_Buzzer(void);
void Stop_Buzzer(void);

#endif /* INC_BUZZER_H_ */
