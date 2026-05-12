#ifndef __SOLEVALVE_H__
#define __SOLEVALVE_H__
#include "main.h"
#define BLOWTIME 2000 //ms


typedef struct{
	GPIO_TypeDef * GPIOGroup;
	uint16_t GPIOPin;
}SoleValve;

extern SoleValve sv[3];
extern __IO uint32_t sv_uwTick;
void solevalve_init();
void valve_open(SoleValve * sv);
void valve_close(SoleValve * sv);
#endif

