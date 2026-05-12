#include "rmYel.h"
#include "Servo.h"
#include "oled.h"


#define STEP0_CY 0
#define STEP1_CY 1
#define STEP2_CY 2
#define STEP3_CY 3
#define STEP4_CY 4
#define STEP5_CY 5
#define STEP6_CY 6



uint8_t ucEnd[1]={0xff};
uint8_t tempflag=0;
__IO uint32_t rmYel_uwTick=0;
rmyel_target target;
CutTime cuttime;
int32_t Knife_Dest=0;
float Knife_Stroke=272.00;//mm
float Threshold_X=15.0f;
float Threshold_Y=40.0f;
float Knife_Y=0;

//347mm 75mm ->stroke 272mm 510mm 7.10r
//23.35mm
//相机中心位置
//现在以相机坐标为坐标
void rmYel_Init(){
	target.x=0;
	target.y=0;
	target.r_x=0;
	target.r_y=0;
	Knife_Y = -1.0*KNIFE_Y_BIAS;
	
	cuttime.uwTick_Fir=0;
	cuttime.uwTick_Sec=0;
	cuttime.State=STEP0_CY;
	
	
	Knife_Dest=-1.00*(0.0+KNIFE_Y_BIAS)/272.00 *ALL_STROKE1 * 0x4000;
	MoveMKS42D_absAxis(MKS42D_1,300,10,Knife_Dest);
	
	MKS42d2_Dest=-1.00 * 0x4000;
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
	if(cuttime.State==STEP0_CY){
		
		if(Knife_Y-target.r_y<=Threshold_Y && Knife_Y-target.r_y>=-1.0 *Threshold_Y){
			if(target.x>=1150 && target.x<=1175){
				Cut_Yel();
				temp++;
			}
		}
	}
}


void rmYel(){
	if(uwTick-rmYel_uwTick < 77){
		return;
	}
	rmYel_uwTick=uwTick;
	
	
	if(cuttime.State == STEP0_CY){
			Target_Update();
      Lock_Update(); 
      if(cuttime.State == STEP0_CY){
        Chasing_Yel();
      }
   }
	
  if(cuttime.State==STEP1_CY){
       cuttime.uwTick_Fir=uwTick;
       cuttime.uwTick_Sec=uwTick+CUTTIME_DOWN+CUT_WAITTIME;
       Set_Servo180Angle(servo180+0,115);
			 cuttime.State=STEP2_CY;
   }
   if(cuttime.State==STEP2_CY){
			if(uwTick<cuttime.uwTick_Sec){
				return;
			}
				 target.x = 0;
				 target.y = 0;
				 target.r_x = 0;
				 target.r_y = 0;
				 Point_Received[0] = 0;
				 Point_Received[1] = 0;
				 Set_Servo180Angle(servo180+0,40);
				 cuttime.State=STEP3_CY;
    }
		if(cuttime.State==STEP3_CY){
			   HAL_UART_Transmit(&huart1,ucEnd,1,1);
         cuttime.State=STEP0_CY;
		}
}

void Cut_Yel(){
	if(cuttime.State!=STEP0_CY){
		return;
	}
	cuttime.State=STEP1_CY;
}

void Chasing_Yel(){
	if(Knife_Y - target.r_y <= HYSTERESIS_THRES && Knife_Y - target.r_y >= -1.0 *HYSTERESIS_THRES){
		return;
	}
	Knife_Dest=-1.00*(target.r_y+KNIFE_Y_BIAS)/272.00 *ALL_STROKE1 * 0x4000;//(target.r_y+KNIFE_Y_BIAS)mm/272mm *7.62 *0x4000
	MoveMKS42D_absAxis(MKS42D_1,300,120,Knife_Dest);
}