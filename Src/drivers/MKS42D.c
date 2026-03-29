#include "MKS42D.h"
#include "gpio.h"
#define MKS42D_DIVISION 16
#define STEPS_PER_REVOLUTION 3200

MKS42D MKS42DGroup[MKS42D_NUM];
uint8_t MKS42D_Index[MKS42D_NUM];
uint8_t MKS42D_Taskx[MKS42D_NUM];
MKS42DSchedule *MKS42D_Schedule[MKS42D_NUM];

MKS42DSchedule MKS42D_0_Schedule[MAX_SCHEDULE];//循环使用//确保任务执行完成再顺行//更简单
MKS42DSchedule MKS42D_1_Schedule[MAX_SCHEDULE];//循环使用
MKS42DSchedule MKS42D_2_Schedule[MAX_SCHEDULE];
MKS42DSchedule MKS42D_3_Schedule[MAX_SCHEDULE];
MKS42DSchedule MKS42D_4_Schedule[MAX_SCHEDULE];
MKS42DSchedule MKS42D_5_Schedule[MAX_SCHEDULE];



void MKS42D_Init(){
	//初始化任务表引索
	MKS42D_Schedule[0]=MKS42D_0_Schedule;
	MKS42D_Schedule[1]=MKS42D_1_Schedule;
	MKS42D_Schedule[2]=MKS42D_2_Schedule;
	MKS42D_Schedule[3]=MKS42D_3_Schedule;
	MKS42D_Schedule[4]=MKS42D_4_Schedule;
	MKS42D_Schedule[5]=MKS42D_5_Schedule;
	//初始化执行引索
	for(int i=0;i<MKS42D_NUM;i++){
		MKS42D_Index[i]=0;
		MKS42D_Taskx[i]=0;
	}

	//初始化任务表
	for(int i=0;i<MAX_SCHEDULE;i++){
		MKS42D_0_Schedule[i].state=EMPTY;
		MKS42D_1_Schedule[i].state=EMPTY;
		MKS42D_2_Schedule[i].state=EMPTY;
		MKS42D_3_Schedule[i].state=EMPTY;
		MKS42D_4_Schedule[i].state=EMPTY;
		MKS42D_5_Schedule[i].state=EMPTY;
	}
	
	//初始化电机硬件
	MKS42DGroup[MKS42D_0].htim=&htim1;
	MKS42DGroup[MKS42D_0].Channel=TIM_CHANNEL_1;
	MKS42DGroup[MKS42D_0].io=RMIOGroup[RMIO_1];
	
	MKS42DGroup[MKS42D_1].htim=&htim1;
	MKS42DGroup[MKS42D_1].Channel=TIM_CHANNEL_2;
	MKS42DGroup[MKS42D_1].io=RMIOGroup[RMIO_3];
	
	MKS42DGroup[MKS42D_2].htim=&htim1;
	MKS42DGroup[MKS42D_2].Channel=TIM_CHANNEL_3;
	MKS42DGroup[MKS42D_2].io=RMIOGroup[RMIO_5];
	
	MKS42DGroup[MKS42D_3].htim=&htim1;
	MKS42DGroup[MKS42D_3].Channel=TIM_CHANNEL_4;
	MKS42DGroup[MKS42D_3].io=RMIOGroup[RMIO_6];
	
	MKS42DGroup[MKS42D_4].htim=&htim8;
	MKS42DGroup[MKS42D_4].Channel=TIM_CHANNEL_1;
	MKS42DGroup[MKS42D_4].io=RMIOGroup[RMIO_7];
	
	MKS42DGroup[MKS42D_5].htim=&htim8;
	MKS42DGroup[MKS42D_5].Channel=TIM_CHANNEL_2;
	MKS42DGroup[MKS42D_5].io=RMIOGroup[RMIO_8];
	
	
}

void MKS42D_AddTask(uint8_t mks_num,float rotatespeed,float rotaten){//转速，行程
	MKS42DSchedule *tempschedule = MKS42D_Schedule[mks_num];
	MKS42D *tempmks = MKS42DGroup+mks_num;

	if(tempschedule[MKS42D_Index[mks_num]].state==EMPTY){
		uint8_t tempdir=1;
		float tempspeed=rotatespeed;
		if(rotatespeed<0){//如果是反转记录为0，速度回为正数
			tempdir=0;
			tempspeed*=-1;
		}
		uint32_t need_time_ms = rotaten/tempspeed*1000;//单位ms

		tempschedule[MKS42D_Index[mks_num]].Dir=tempdir;
		tempschedule[MKS42D_Index[mks_num]].speed=tempspeed;
		tempschedule[MKS42D_Index[mks_num]].Needtime=need_time_ms;
		tempschedule[MKS42D_Index[mks_num]].state=WAITING;
	}
	MKS42D_Index[mks_num]= (MKS42D_Index[mks_num]+1)%128;
}


void RunMKS42D(){//放在定时器中断中执行
	for(int i=0;i<MKS42D_NUM;i++){

		MKS42D *tempmks = MKS42DGroup+i;
		uint8_t tempTaskx=MKS42D_Taskx[i];
		MKS42DSchedule *tempschedule = MKS42D_Schedule[i]+tempTaskx;
		GPIO_RMIO tempio = tempmks->io;
		
		if(tempschedule->state==EMPTY){
			if(__HAL_TIM_GetCompare(tempmks->htim,tempmks->Channel)>0){
				setPWM(tempmks->htim,tempmks->Channel,2000,0);//如果是接下来任务是空的那么停转
			}
		}
		else if(tempschedule->state==WAITING){
			__IO uint32_t uwTick_End=tempschedule->Needtime+uwTick;
			tempschedule->Endtime=uwTick_End;
			tempschedule->state=RUNNING;
			float tempspeed=tempschedule->speed;
			uint32_t tempfreq=tempspeed*STEPS_PER_REVOLUTION;
			if(tempschedule->Dir==1){
				tempio.GPIOPinState=GPIO_PIN_SET;
				RMIO_Update();
			}
			else if(tempschedule->Dir==0){
				tempio.GPIOPinState=GPIO_PIN_RESET;
				RMIO_Update();
			}
			setPWM(tempmks->htim,tempmks->Channel,tempfreq,0.2);
		}
		else if(tempschedule->state==RUNNING){
			if(tempschedule->Endtime > uwTick){
				tempschedule->state=RUNNING;
			}
			else if(tempschedule->Endtime <= uwTick){
				//tempschedule->state=FINNESHED;
				//结束后置空
				tempschedule->state=EMPTY;
				tempschedule->Needtime=0;
				tempschedule->speed=0;

				MKS42D_Taskx[i]=(MKS42D_Taskx[i]+1)%MAX_SCHEDULE;//拨到下一个任务
			}
		}
	}
}
