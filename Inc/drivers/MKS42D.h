#ifndef __MKS42D_H__
#define __MKS42D_H__
#include "main.h"
#include "gpio.h"
#define MKS42D_DIVISION 16
#define STEPS_PER_REVOLUTION 3200
#define MKS42D_1 0x01


extern uint8_t ucCANTrans[20];
extern __IO uint32_t mks42d_uwTick;
extern int64_t mks42d_encoder[1];

//-8,388,608 µ½ 8,388,607

void MoveMKS42D(uint8_t num,uint16_t speed,uint8_t acc,int32_t absPulse);
void MKS42D_Proc();
#endif