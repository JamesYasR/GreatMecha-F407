#include "init.h"
#include "MKS42D.h"
#include "usart.h"
#include "string.h"
void InitAll(void){
	HAL_Init();
  SystemClock_Config();
	
  GPIO_Init();
	
  DMA_Init();
  USART1_UART_Init();
	USART6_UART_Init();
	
	memset(ucBuffer1,0,sizeof(ucBuffer1));
	memset(ucRecei1,0,sizeof(ucRecei1));
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei1,ucBuffSize);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	
	
	memset(ucBuffer6,0,sizeof(ucBuffer6));
	memset(ucRecei6,0,sizeof(ucRecei6));
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6,ucRecei6,ucBuffSize);
  __HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);
	
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