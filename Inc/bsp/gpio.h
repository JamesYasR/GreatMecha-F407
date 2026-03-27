
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define RMIO_1 0
#define RMIO_3 1
#define RMIO_5 2
#define RMIO_6 3
#define RMIO_7 4
#define RMIO_8 5


void GPIO_Init(void);

typedef struct{
	GPIO_TypeDef * GPIOGroup;
	uint16_t GPIOPin;
	GPIO_PinState GPIOPinState;
}GPIO_RMIO;

extern GPIO_RMIO RMIOGroup[6];
void RMIO_Update();
void RMIO_Init(void);
void RMIO_WritePin(GPIO_RMIO * RMPIN,GPIO_PinState state);
#ifdef __cplusplus
}
#endif
#endif 

