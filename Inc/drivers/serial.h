#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#define STARTFLAG 0x80
#define ENDFLAG 0x81
#define SPLITFLAG 0x82
#define SENDPOINTFLOATFLAG 0x83
#define MKS42DTASKFLAG 0x84
#define SERVOTASKFLAG 0x85

extern __IO uint32_t serial_uwTick;
void Serial_proc();
uint8_t process_Point(uint8_t *str, uint16_t len);
extern float Point_Received[2];
extern uint16_t error_serial[1];
extern uint8_t TARGETUPDATE_FLAG;
extern uint8_t temp;

#endif


