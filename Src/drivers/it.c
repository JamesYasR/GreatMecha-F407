#include "it.h"
#include "MKS42D.h"
#include "key.h"
#include "MKS42D.h"
#include "dma.h"
#include "oled.h"
#include "Servo.h"
#include "rmYel.h"

uint32_t TIM6_Tick=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//
	if(htim->Instance==TIM6){
		TIM6_Tick++;
		
	  RunMKS42D();
		RunServo();
		
		if(KEY_DOWN==1){
			HAL_UART_Transmit(&huart1,ucTrans1,strlen((char *)ucTrans1),2);
			Cut_Yel();
			KEY_DOWN=0;
		}
	}
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	if(huart->Instance==USART1){
		IS_UART_RECEIVED[0]=RECEIVED;
		
		memcpy(ucBuffer1,ucRecei1,sizeof(ucRecei1));
		memset(ucRecei1,0,sizeof(ucRecei1));
		uart_recNum[0]++;
		ucBuffer_len[0]=Size;
		
		//HAL_UART_Transmit(&huart1,ucBuffer1,ucBuffer_len[0],10);
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei1,ucBuffSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);

	}
	
	if(huart->Instance==USART6){
		IS_UART_RECEIVED[1]=RECEIVED;
		
		memcpy(ucBuffer6,ucRecei6,sizeof(ucRecei6));
		memset(ucRecei6,0,sizeof(ucRecei6));
		uart_recNum[1]++;
		ucBuffer_len[1]=Size;
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6,ucRecei6,ucBuffSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);
	}
	
}


//错误处理非常重要
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        error_recNum[0]++;    
        
        // 清除所有错误标志
        uint32_t isrflags = READ_REG(huart->Instance->SR);
        uint32_t cr1its = READ_REG(huart->Instance->CR1);
        
        // 帧错误
        if (isrflags & USART_SR_FE)
        {
            __HAL_UART_CLEAR_FEFLAG(huart);
        }
        // 噪声错误
        if (isrflags & USART_SR_NE)
        {
            __HAL_UART_CLEAR_NEFLAG(huart);
        }
        // 过载错误
        if (isrflags & USART_SR_ORE)
        {
            __HAL_UART_CLEAR_OREFLAG(huart);
        }
        // 奇偶校验错误
        if (isrflags & USART_SR_PE)
        {
            __HAL_UART_CLEAR_PEFLAG(huart);
        }
        
        // 重新启动DMA接收
        HAL_UART_DMAStop(huart);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, ucRecei1, ucBuffSize);
        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
        
        
        huart->ErrorCode = HAL_UART_ERROR_NONE;
    }
		if (huart->Instance == USART6)
    {
        error_recNum[1]++;    
        
        // 清除所有错误标志
        uint32_t isrflags = READ_REG(huart->Instance->SR);
        uint32_t cr1its = READ_REG(huart->Instance->CR1);
        
        // 帧错误
        if (isrflags & USART_SR_FE)
        {
            __HAL_UART_CLEAR_FEFLAG(huart);
        }
        // 噪声错误
        if (isrflags & USART_SR_NE)
        {
            __HAL_UART_CLEAR_NEFLAG(huart);
        }
        // 过载错误
        if (isrflags & USART_SR_ORE)
        {
            __HAL_UART_CLEAR_OREFLAG(huart);
        }
        // 奇偶校验错误
        if (isrflags & USART_SR_PE)
        {
            __HAL_UART_CLEAR_PEFLAG(huart);
        }
        
        // 重新启动DMA接收
        HAL_UART_DMAStop(huart);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, ucRecei6, ucBuffSize);
        __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
        
        
        huart->ErrorCode = HAL_UART_ERROR_NONE;
    }
}