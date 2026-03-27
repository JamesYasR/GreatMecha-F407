#ifndef __TIM_H__
#define __TIM_H__
#include "main.h"


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim6;


void TIM1_Init(void);
void TIM8_Init(void);
void TIM6_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void setPWM(TIM_HandleTypeDef * htim, uint32_t channel,uint32_t freq, float duty);
#endif


