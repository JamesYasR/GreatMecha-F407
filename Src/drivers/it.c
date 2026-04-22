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
		
		RunServo();
		
		if(KEY_DOWN==1){
			HAL_UART_Transmit(&huart1,ucTrans1,strlen((char *)ucTrans1),2);
			
			MoveMKS42D(MKS42D_1,300,4,7.62*STEPS_PER_REVOLUTION);
			//Cut_Yel();
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

//++
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t rx_data[8];
    
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rx_data) != HAL_OK) {
        return;
    }
    if(RxHeader.StdId == MKS42D_1 && RxHeader.DLC == 8) {
        if(rx_data[0] == 0x31) {  // 功能码匹配
            
            // 可选：验证CRC校验和
            uint8_t crc_calc = RxHeader.StdId;
            for(int i = 0; i < 7; i++) {
                crc_calc += rx_data[i];
            }
            if(crc_calc != rx_data[7]) {
                // CRC校验失败，丢弃此帧
                return;
            }
            
            // 组合6字节编码器值 (高位在前)
            int64_t encoder_val = 0;
            for(int i = 0; i < 6; i++) {
                encoder_val = (encoder_val << 8) | rx_data[i+1];
            }
            
            // 处理48位有符号数：如果第47位为1，则为负数
            if(encoder_val & (1ULL << 47)) {
                // 符号扩展到64位
                encoder_val |= 0xFFFF000000000000ULL;
            }
            
            mks42d_encoder[0] = encoder_val;
        }
    }
}
//++

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