#ifndef __RMYEL_H__
#define __RMYEL_H__
#include "main.h"
#include "Servo.h"
#include "Serial.h"
#include "MKS42D.h"

#define IMAGE_H 1280//pixels
#define IMGAE_V 720//pixels
#define STROKE_PER_REVOLUTION 20.00f//mm

#define NOT_INIT 0
#define WAIT_CL 1
#define IS_CLD 2

#define CUTTIME_DOWN 190 //ms
#define CUTTIME_UP 240 //ms
#define CUT_WAITTIME 5000 //ms
#define CUTSPEED 0.6f

#define CAM_WIDTH 1280 //pixel
#define CAM_HEIGHT 720

#define REAL_WIDTH 320.00f //mm
#define REAL_HEIGHT 180.00f //mm
#define KNIFE_X REAL_WIDTH/2+294.55f //mm

extern __IO uint32_t rmYel_uwTick;
extern double r_per_mm;
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

typedef struct{
	float stroke;
	float position;
	float bias_y;
}Platform;


void rmYel_Init();
extern rmyel_target target;
extern CutTime cuttime;
extern Platform platform;
void Target_Update();
void rmYel_proc();

void Cut_Yel();
void Chasing_Yel();

#endif