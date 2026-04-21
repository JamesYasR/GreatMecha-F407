#ifndef __SERVO_H__
#define __SERVO_H__
#include "main.h"
#include "tim.h"
#define SERVO360NUM 1
#define SERVO180NUM 1
#define IDLE 0
#define WAIT 1
#define BUSY 2
#define SERVO180TIME 50
extern __IO uint32_t servo_uwTick;

typedef struct{
	TIM_HandleTypeDef * htim;
	uint32_t Channel;
	float Speed;
	uint32_t StopTime;
	uint8_t State;
}Servo360;

typedef struct{
	TIM_HandleTypeDef * htim;
	uint32_t Channel;
	float Angle;
	uint32_t StopTime;
	uint8_t State;
}Servo180;

extern Servo360 servo360[SERVO360NUM];
extern Servo180 servo180[SERVO180NUM];
void Servo_Init();

void Set_Servo180Angle(Servo180 * servo,float angle);
void Set_Servo360Speed(Servo360 *servo,float speed,uint32_t time);

void RunServo();


#endif