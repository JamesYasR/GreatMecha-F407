#include "it.h"
#include "MKS42D.h"
#include "key.h"
#include "MKS42D.h"

uint32_t TIM6_Tick=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//
	if(htim->Instance==TIM6){
		TIM6_Tick++;
		
	  RunMKS42D();
		
		
		if(KEY_DOWN==1){
			HAL_UART_Transmit(&huart1,ucTrans,strlen((char *)ucTrans),1000);
			
			KEY_DOWN=0;
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance==USART1){
		memcpy(ucTrans,ucRecei,Size);
		memset(ucRecei,0,sizeof(ucRecei));
		HAL_UART_Transmit(&huart1,ucTrans,Size,1000);
	}
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei,ucBuffSize);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
}
