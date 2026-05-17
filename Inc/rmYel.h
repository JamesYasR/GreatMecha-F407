#ifndef __RMYEL_H__
#define __RMYEL_H__
#include "main.h"
#include "Servo.h"
#include "Serial.h"
#include "MKS42D.h"

#define IMAGE_H 1280//pixels
#define IMGAE_V 720//pixels

#define NOT_INIT 0
#define WAIT_CL 1
#define IS_CLD 2

#define CUTTIME_DOWN 200  //ms
#define CUTTIME_UP 800 //ms
#define CUT_WAITTIME 30 //ms
#define CUTSPEED 0.6f

#define CAM_WIDTH 1280 //pixel
#define CAM_HEIGHT 720

#define REAL_WIDTH 547.5144f //mm
#define REAL_HEIGHT 308.1542f //mm
// ???????? 248.00f+273.7572f //mm=521.7572

#define KNIFE_Y_BIAS 3.5729 //14.5729mm
#define HYSTERESIS_THRES 10.0f
extern __IO uint32_t rmYel_uwTick;
extern int32_t Knife_Dest;
extern float Knife_Y;
typedef struct{
	float x;
	float y;
	float r_x;
	float r_y;
}rmyel_target;

typedef struct{
	uint32_t uwTick_Fir;
	uint32_t uwTick_Sec;
	uint8_t State;
}CutTime;



void rmYel_Init();
extern rmyel_target target;
extern CutTime cuttime;
void Target_Update();
void rmYel_proc();

void Cut_Yel();
void Chasing_Yel();
void rmYel();
#endif