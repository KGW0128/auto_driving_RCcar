/*
 * RC_moter.h
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */

#ifndef INC_RC_MOTER_H_
#define INC_RC_MOTER_H_

//모터 핀 번호

//왼쪽
#define MOTOR_L_IN4_PIN   GPIO_PIN_1
#define MOTOR_L_IN3_PIN   GPIO_PIN_15

//오른쪽
#define MOTOR_R_IN2_PIN   GPIO_PIN_14
#define MOTOR_R_IN1_PIN   GPIO_PIN_13

//모터가 연결된 포트
#define MOTOR_GPIO_PORT   GPIOB


void Moter_Init();

void Moter_Advance();   //전진
void Moter_Junior();    //후진
void Moter_Left();      //좌회전
void Moter_Right();     //우회전

void Moter_Stop();     //정지


#endif /* INC_RC_MOTER_H_ */
