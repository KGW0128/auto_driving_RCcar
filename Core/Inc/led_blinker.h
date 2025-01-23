/*
 * led_blinker.h
 *
 *  Created on: Jan 13, 2025
 *      Author: user
 */

#ifndef INC_LED_BLINKER_H_
#define INC_LED_BLINKER_H_

#include <stdint.h>  // uint16_t, uint8_t 사용을 위한 추가
#include "main.h"    // STM32 관련 헤더
#include "gpio.h"    // GPIO 관련 헤더
#include "tim.h"     // 타이머 관련 헤더




void LED_All_Timer_Interrupt(void);
void LED_Left_Timer_Interrupt(void);
void LED_Right_Timer_Interrupt(void);

void Start_Blinker(void);
void Stop_Blinker(void);

#endif /* INC_LED_BLINKER_H_ */
