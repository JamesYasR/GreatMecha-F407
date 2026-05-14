#ifndef __MKS42D_H__
#define __MKS42D_H__
#include "main.h"
#include "gpio.h"
#define MKS42D_DIVISION 16
#define STEPS_PER_REVOLUTION 3200
#define AXIS_PER_REVOLUTION 0x4000
#define ALL_STROKE1 7.55f
#define ALL_STROKE2 9.45f
#define MKS42D_1 0x01
#define MKS42D_2 0x02

extern uint8_t ucCANTrans[20];
extern __IO uint32_t mks42d_uwTick;
extern int32_t MKS42d2_Dest;

typedef struct{
	int64_t axis;
	int16_t speed;
	int32_t pulse;
}MKS42D_INFO;


//-8,388,608 µ½ 8,388,607
extern MKS42D_INFO MKS42D1_INFO;
extern MKS42D_INFO MKS42D2_INFO;

void MoveMKS42D_absPluse(uint8_t num,uint16_t speed, uint8_t acc,int32_t absPulse);
void MoveMKS42D_absAxis(uint8_t num, uint16_t speed, uint8_t acc,int32_t absAxis);
void SetMKS42D_Speed(uint8_t num, uint8_t dir,uint16_t speed, uint8_t acc);

void MKS42D_Proc();
void MKS42D_Init();
#endif