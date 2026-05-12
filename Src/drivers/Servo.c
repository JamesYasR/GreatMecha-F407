#include "Servo.h"
#include "usart.h"
#include "stdio.h"

__IO uint32_t servo_uwTick=0;
Servo180 servo180[SERVO180NUM];


void Servo_Init(){
	servo180[0].htim=&htim8;
	servo180[0].Channel=TIM_CHANNEL_1;
	servo180[0].Angle=0.0f;
}

void Set_Servo180Angle(Servo180 * servo,float angle){//不可调速有状态
	if(angle<0.00f || angle > 180.00f){
		return;
	}
	servo->Angle=angle;
	setPWM(servo->htim,servo->Channel,50,(servo->Angle)/180.0f * 0.1 + 0.025);
}
