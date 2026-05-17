#include "rmYel.h"
#include "Servo.h"
#include "oled.h"


#define STEP0_CY 0
#define STEP1_CY 1
#define STEP1_1CY 7
#define STEP2_CY 2
#define STEP3_CY 3
#define STEP4_CY 4
#define STEP5_CY 5
#define STEP6_CY 6

#define THRES 1140

#define DELAY 20
uint8_t ucEnd[1]={0xff};
uint8_t tempflag=0;
__IO uint32_t rmYel_uwTick=0;
__IO uint32_t last_rmYel_uwTick=0;

rmyel_target target;
int32_t Knife_Dest=0;
float Knife_Stroke=272.00;//mm
float Threshold_X=15.0f;
float Threshold_Y=40.0f;
float Knife_Y=0;
float last_x=1220.0f;

//347mm 75mm ->stroke 272mm 510mm 7.10r
//23.35mm
//相机中心位置
//现在以相机坐标为坐标

uint8_t rmYel_state=STEP0_CY;


void rmYel_Init(){
	target.x=0;
	target.y=0;
	target.r_x=0;
	target.r_y=0;
	Knife_Y = -1.0*KNIFE_Y_BIAS;
	
	
	Knife_Dest=-1.00*(0.0+KNIFE_Y_BIAS)/272.00 *ALL_STROKE1 * 0x4000;
	MoveMKS42D_absAxis(MKS42D_1,300,10,Knife_Dest);
	
	MKS42d2_Dest=-1.5 * 0x4000;
	MoveMKS42D_absAxis(MKS42D_2,600,10,MKS42d2_Dest);
}

void Target_Update(){
	target.x=Point_Received[0];
	target.y=Point_Received[1];
	target.r_x=target.x/CAM_WIDTH * REAL_WIDTH;//换算实时实际坐标
	target.r_y=target.y/CAM_HEIGHT * REAL_HEIGHT;
	
	Knife_Y=-1.0 *(double)MKS42D1_INFO.pulse/STEPS_PER_REVOLUTION/ALL_STROKE1 * 272.00f - KNIFE_Y_BIAS;//想明白了，这里计算的问题 //可以得到刀的实时坐标
}

void Lock_Update(){
	if(rmYel_state==STEP0_CY){
		if(Knife_Y-target.r_y<=Threshold_Y && Knife_Y-target.r_y>=-1.0 *Threshold_Y){
			if(target.x>=THRES && target.x<=THRES+15){
				//if(target.x>last_x+5){
					//return;
				//}
				Cut_Yel();
				last_x = target.x;
			}
		}
	}
}


void rmYel(){
	if(uwTick-rmYel_uwTick < 50){
		return;
	}
	rmYel_uwTick=uwTick;
	
	
	if(rmYel_state == STEP0_CY){
			Target_Update();
      Lock_Update(); 
      if(rmYel_state == STEP0_CY){
        Chasing_Yel();
      }
   }
	
  else if(rmYel_state ==STEP1_CY){
       last_rmYel_uwTick =uwTick;
			 rmYel_state =STEP1_1CY;
   }

	 else if(rmYel_state ==STEP1_1CY){
			if(uwTick-last_rmYel_uwTick < DELAY){
				return;
			}
			 Set_Servo180Angle(servo180+0,133);
       last_rmYel_uwTick =uwTick;
			 rmYel_state =STEP2_CY;
   }

   else if(rmYel_state ==STEP2_CY){
			if(uwTick>=last_rmYel_uwTick+CUTTIME_DOWN+CUT_WAITTIME){
				 Set_Servo180Angle(servo180+0,40);
				 rmYel_state =STEP3_CY;
				 last_rmYel_uwTick = uwTick;
			}
    }
		else if(rmYel_state ==STEP3_CY){
			   HAL_UART_Transmit(&huart1,ucEnd,1,1);
				 rmYel_state =STEP4_CY;
         last_rmYel_uwTick = uwTick;
				 
		}
		else if(rmYel_state ==STEP4_CY){
			  if(uwTick>=last_rmYel_uwTick+40){//让上位机有充足时间切换目标
					 //状态解锁将会触发Lock_Update，所以提前清空缓存，防止一个目标连续被执行
					 target.x = 0;
					 target.y = 0;
					 target.r_x = 0;
					 target.r_y = 0;
					 Point_Received[0] = 0;
					 Point_Received[1] = 0;
					 rmYel_state =STEP0_CY;
					 last_rmYel_uwTick = 0;
				}
		}
}

void Cut_Yel(){
	if(rmYel_state !=STEP0_CY){
		return;
	}
	rmYel_state =STEP1_CY;
}

void Chasing_Yel(){
	if(Knife_Y - target.r_y <= HYSTERESIS_THRES && Knife_Y - target.r_y >= -1.0 *HYSTERESIS_THRES){
		return;
	}
	Knife_Dest=-1.00*(target.r_y+KNIFE_Y_BIAS)/272.00 *ALL_STROKE1 * 0x4000;//(target.r_y+KNIFE_Y_BIAS)mm/272mm *7.62 *0x4000
	MoveMKS42D_absAxis(MKS42D_1,300,120,Knife_Dest);
}