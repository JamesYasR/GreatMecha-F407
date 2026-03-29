#include "init.h"
#include "MKS42D.h"
void InitAll(void){
	HAL_Init();
  SystemClock_Config();
	
  GPIO_Init();
	
  DMA_Init();
  USART1_UART_Init();
	
	TIM1_Init();
	TIM8_Init();
	TIM6_Init();

	HAL_TIM_Base_Start_IT(&htim6);
	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	
	RMIO_Init();
	MKS42D_Init();
}