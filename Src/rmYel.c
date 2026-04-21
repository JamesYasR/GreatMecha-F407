#include "rmYel.h"
#include "Servo.h"

uint8_t ucEnd[1]={0xff};

__IO uint32_t rmYel_uwTick=0;
rmyel_target target;
CutTime cuttime;
float knife_xerror=0.0f;

void rmYel_Init(){
	target.x=0;
	target.y=0;
	cuttime.uwTick_Fir=0;
	cuttime.uwTick_Sec=0;
	cuttime.State=IDLE;
}

void Target_Update(){
	if(TARGETUPDATE_FLAG==0){
		return;	
	}
	TARGETUPDATE_FLAG=0;
	target.x=Point_Received[0];
	target.y=Point_Received[1];
	target.r_x=target.x/CAM_WIDTH * REAL_WIDTH;
	target.r_y=target.y/CAM_HEIGHT * REAL_HEIGHT;
	
}


void rmYel(){
	if(uwTick-rmYel_uwTick < 50){
		return;
	}
	rmYel_uwTick=uwTick;
	Target_Update();
	
	if(cuttime.State==WAIT){
		cuttime.State=BUSY;
		Set_Servo180Angle(servo180+0,170.0);
	}
	else if(cuttime.State==BUSY){
		if(uwTick>=cuttime.uwTick_Sec){
			cuttime.State=IDLE;
			Set_Servo180Angle(servo180+0,0.0);
		}
	}//¥¶¿Ìµ∂æﬂ
	
}

void Cut_Yel(){
	if(cuttime.State!=IDLE){
		return;
	}
	cuttime.State=WAIT;
	cuttime.uwTick_Fir=uwTick;
	cuttime.uwTick_Sec=uwTick+CUTTIME_DOWN+CUT_WAITTIME;
}

void Chasing_Yel(){
	knife_xerror=KNIFE_X-target.r_x;
	if(knife_xerror<=0.0f){
		HAL_UART_Transmit(&huart1,ucEnd,1,1);
		return;
	}
}

