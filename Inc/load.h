#ifndef __LOAD_H__
#define __LOAD_H__
#include "main.h"

extern __IO uint32_t load_uwTick;
extern __IO uint32_t tray_uwTick;
extern uint8_t load_state;
extern uint8_t tray_state;


extern uint8_t CSD_FLAG;
void load_proc();
void Tray_Move();
#endif