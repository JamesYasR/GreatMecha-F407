#ifndef __SERVO_H__
#define __SERVO_H__
#include "main.h"
#include "tim.h"
#define SERVO360NUM 1
#define SERVO180NUM 1
#define IDLE 0
#define WAIT 1
#define BUSY 2




extern __IO uint32_t servo_uwTick;

typedef struct{
	TIM_HandleTypeDef * htim;
	uint32_t Channel;
	float Angle;
}Servo180;

extern Servo180 servo180[SERVO180NUM];
void Servo_Init();

void Set_Servo180Angle(Servo180 * servo,float angle);

void RunServo();


#endif