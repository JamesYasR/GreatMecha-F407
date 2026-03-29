#include "it.h"
#include "MKS42D.h"
#include "key.h"
#include "MKS42D.h"

uint32_t TIM6_Tick=0;
uint8_t ucTrans[4]="test";
uint8_t ucRecei[4];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//
	if(htim->Instance==TIM6){
		TIM6_Tick++;
		
	  RunMKS42D();
		
		
		if(KEY_DOWN==1){
			HAL_UART_Transmit(&huart1,ucTrans,4,1000);
			MKS42D_AddTask(MKS42D_0,1,1);
			KEY_DOWN=0;
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance==USART1){
		memcpy(ucTrans,ucRecei,4);
		HAL_UART_Transmit(&huart1,ucTrans,4,1000);
	}
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei,50);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
}
