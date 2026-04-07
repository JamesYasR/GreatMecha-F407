#include "it.h"
#include "MKS42D.h"
#include "key.h"
#include "MKS42D.h"
#include "dma.h"

uint32_t TIM6_Tick=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//
	if(htim->Instance==TIM6){
		TIM6_Tick++;
		
	  RunMKS42D();
		
		
		if(KEY_DOWN==1){
			HAL_UART_Transmit(&huart6,ucTrans6,strlen((char *)ucTrans6),1000);
			
			KEY_DOWN=0;
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	if(huart->Instance==USART1){
		IS_UART_RECEIVED[0]=RECEIVED;

		ucBuffer_len[0]=Size;
		

	}
	
	if(huart->Instance==USART6){
		IS_UART_RECEIVED[1]=RECEIVED;

		ucBuffer_len[1]=Size;
		HAL_UART_Transmit(&huart6,ucRecei6,Size,1);
		

	}
	
}





void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART1){
        usart6_tx_busy = 0; 
    }
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART6){
        usart6_tx_busy = 0;  // 发送出错，清除忙标志
    }
    if(huart->Instance == USART1)
    {
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)
        {
						HAL_UART_Transmit(&huart6,(uint8_t *)"ore",3,10);
            __HAL_UART_CLEAR_OREFLAG(&huart1);
            
            HAL_UART_DMAStop(&huart1);
            
            memset(&huart1, 0, sizeof(UART_HandleTypeDef));
						memset(&hdma_usart1_rx, 0, sizeof(DMA_HandleTypeDef));
						HAL_UART_MspDeInit(&huart1);  // 反初始化
						USART1_UART_Init();        // 重新初始化
						DMA_Init();     
            
            // 重新启动DMA接收
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, ucRecei1, ucBuffSize);
            __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
            
        }
    }
}