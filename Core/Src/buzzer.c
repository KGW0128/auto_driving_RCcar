/*
 * buzzer.c
 *
 *  Created on: Jan 13, 2025
 *      Author: user
 */

#include "buzzer.h"


uint16_t song[]=
    {
    		Do,Do,Mi,Sol,Do,Mi,Sol,La,La,La,Sol,
        Fa,Fa,Fa,Mi,Mi,Mi,Re,Re,Re,Do
    };


uint8_t sound_i = 0;



void Buzzer_Timer_Interrupt(void)
{
    if (++sound_i >= sizeof(song) / sizeof(song[0]))
    {
        sound_i = 0;  // 배열 끝에 도달하면 초기화
    }
}


void Start_Buzzer(void)
{
    HAL_TIM_Base_Start_IT(&htim11);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
    TIM5->PSC = song[sound_i];  // 음계 설정
    TIM5->CCR1 = 900;           // 듀티 사이클 설정
}


void Stop_Buzzer(void)
{
    HAL_TIM_Base_Stop_IT(&htim11);
    TIM5->CCR1 = 0;  // PWM 출력 완전히 종료
    HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
    sound_i = 0;
}
