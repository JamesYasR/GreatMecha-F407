#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#define STARTFLAG 0x80
#define ENDFLAG 0x81
#define SPLITFLAG 0x82
#define SENDPOINTFLOATFLAG 0xA1

extern __IO uint32_t serial_uwTick;
void serial_proc();
uint8_t ascii2digit(uint8_t ascii);
void process_Point(uint8_t * str,uint16_t len);
extern float Point_Received[2];
#endif


