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
	
	RMIO_Init();
	MKS42D_Init();
}