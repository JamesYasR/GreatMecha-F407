#include "main.h"
#include "init.h"
#include "stdio.h"
#include "string.h"
//////////////////////////FLAG
uint8_t U1RECEIVED = 0;

//////////////////////////GLOBAL VARIABLE
uint8_t ucTrans[4]={0x01,0x02,0x03,0x04};
uint8_t ucRecei[50];

int main(void)
{
	InitAll();

  //HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei,50);
  //__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	//HAL_UART_Transmit(&huart1,ucTrans,50,1000);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	setPWM(&htim8,TIM_CHANNEL_3,3200,0.2);
	
  while (1)
  {
		HAL_UART_Transmit(&huart1,ucTrans,4,1000);
		HAL_Delay(500);
  }

}
////////////////////////////////////////////////////////////////it   handler///////////////////////////////////////////////////////////////////////////////////
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//	if(huart->Instance==USART1){
//		U1RECEIVED=1;
//		//memcpy(ucTrans,ucRecei,50);
//		HAL_UART_Transmit(&huart1,ucTrans,4,1000);
//	}
//	
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei,50);
//  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
//}
////////////////////////////////////////////////////////////////error handler///////////////////////////////////////////////////////////////////////////////////
void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

