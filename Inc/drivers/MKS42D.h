#ifndef __MKS42D_H__
#define __MKS42D_H__
#include "main.h"
#include "tim.h"
#include "gpio.h"
#define MKS42D_0 0
#define MKS42D_1 1
#define MKS42D_2 2
#define MKS42D_3 3


#define MAX_SCHEDULE 128
#define MKS42D_NUM 4

enum MotorState{
	BUSY_=0,
	IDLE_
};

enum ScheduleState{
	RUNNING_=0,
	FINNESHED_,
	WAITING_,
	EMPTY_
};

typedef struct{
	TIM_HandleTypeDef * htim;
	uint32_t Channel;
	GPIO_RMIO *io;
	uint32_t Steps;
}MKS42D;

typedef struct{
	float speed;
	uint32_t Needtime;
	uint32_t Endtime;
	uint8_t Dir;
	enum ScheduleState state;
}MKS42DSchedule;																																																																																																										

void MKS42D_Init();
void RunMKS42D();
void Schedule_Reset(MKS42DSchedule * schedule);
void MKS42D_AddTask(uint8_t mks_num,float rotatespeed,float rotaten);

extern MKS42D MKS42DGroup[MKS42D_NUM];
extern MKS42DSchedule MKS42D_0_Schedule[MAX_SCHEDULE];
extern MKS42DSchedule MKS42D_1_Schedule[MAX_SCHEDULE];
extern MKS42DSchedule MKS42D_2_Schedule[MAX_SCHEDULE];
extern MKS42DSchedule MKS42D_3_Schedule[MAX_SCHEDULE];

extern uint8_t MKS42D_Index[MKS42D_NUM];
extern uint8_t MKS42D_Taskx[MKS42D_NUM];
extern MKS42DSchedule *MKS42D_Schedule[MKS42D_NUM];

#endif