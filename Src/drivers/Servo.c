#include "Servo.h"
#include "usart.h"
#include "stdio.h"

__IO uint32_t servo_uwTick=0;
Servo360 servo360[SERVO360NUM];
Servo180 servo180[SERVO180NUM];


void Servo_Init(){
	servo180[0].htim=&htim8;
	servo180[0].Channel=TIM_CHANNEL_1;
	servo180[0].Angle=0.0f;
	servo180[0].StopTime=0;
	servo180[0].State=IDLE;
	
	servo360[0].htim=&htim8;
	servo360[0].Channel=TIM_CHANNEL_2;
	servo360[0].Speed=0.0f;
	servo360[0].StopTime=0;
	servo360[0].State=IDLE;
}

void Set_Servo180Angle(Servo180 * servo,float angle){//不可调速有状态
	if(angle<0.00f || angle > 180.00f){
		return;
	}
	servo->State=WAIT;
	servo->Angle=angle;
	servo->StopTime=uwTick+SERVO180TIME;
}

void Set_Servo360Speed(Servo360 *servo,float speed,uint32_t time){//65535ms->65.5s //speed:-1.0~1.0//duty->0.025-0.075-12.5//7.5stop//0.05
	if(speed<-1.0 || speed > 1.0){
		return;
	}
	if(servo->State!=IDLE){
		return;
	}
	servo->Speed=speed;
	servo->State=WAIT;
	servo->StopTime=uwTick+time;
}

void RunServo(){
	if(uwTick-servo_uwTick < 10){
		return;
	}
	servo_uwTick=uwTick;
	
	for(int i=0;i<SERVO180NUM;i++){
		if((servo180+i)->State==WAIT){
			(servo180+i)->State=BUSY;
			 setPWM((servo180+i)->htim,(servo180+i)->Channel,50,(servo180+i)->Angle/180*0.1+0.025);
		}
		else if((servo180+i)->State==BUSY){
			if(uwTick>=(servo180+i)->StopTime){
				(servo180+i)->State=IDLE;
			}
		}
		else if((servo180+i)->State==IDLE){
			continue;
		}
	}//处理180度舵机
	
	for(int i=0;i<SERVO360NUM;i++){
		if((servo360+i)->State==WAIT){
			(servo360+i)->State=BUSY;
			 setPWM((servo360+i)->htim,(servo360+i)->Channel,50,0.075-(servo360+i)->Speed*0.05);
		}
		else if((servo360+i)->State==BUSY){
			if(uwTick>=(servo360+i)->StopTime){
				(servo360+i)->State=IDLE;
				setPWM((servo360+i)->htim,(servo360+i)->Channel,50,0.075);
			}
		}
		else if((servo360+i)->State==IDLE){
			continue;
		}
	}//处理360度舵机
	
	
	
	
}