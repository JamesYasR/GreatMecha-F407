#ifndef __MKS42D_H__
#define __MKS42D_H__
#include "main.h"
#include "tim.h"
#include "gpio.h"

#define MAX_SCHEDULE 128
#define MKS42D_NUM 2
enum MotorState{
	BUSY=0,
	IDLE
};

enum ScheduleState{
	RUNNING=0,
	FINNESHED,
	WAITING,
	EMPTY
};

typedef struct{
	TIM_HandleTypeDef * htim;
	uint32_t Channel;
	GPIO_RMIO io;
}MKS42D;

typedef struct{
	float speed;
	uint32_t Needtime;
	uint32_t Endtime;
	uint8_t Dir;
	enum ScheduleState state;
}MKS42DSchedule;

void MKS42D_Init();

extern MKS42D MKS42DGroup[MKS42D_NUM];
extern MKS42DSchedule MKS42D_0_Schedule[MAX_SCHEDULE];
extern MKS42DSchedule MKS42D_1_Schedule[MAX_SCHEDULE];
extern uint8_t MKS42D_Index[MKS42D_NUM];
extern uint8_t MKS42D_Taskx[MKS42D_NUM];
extern MKS42DSchedule *MKS42D_Schedule[MKS42D_NUM];

#endif