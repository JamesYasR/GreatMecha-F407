#include "gpio.h"

GPIO_RMIO RMIOGroup[4];
void RMIO_Init(){
	RMIOGroup[RMIO_1].GPIOGroup=GPIOB;
	RMIOGroup[RMIO_1].GPIOPin=GPIO_PIN_12;
	RMIOGroup[RMIO_1].GPIOPinState=GPIO_PIN_RESET;
	
	RMIOGroup[RMIO_3].GPIOGroup=GPIOB;
	RMIOGroup[RMIO_3].GPIOPin=GPIO_PIN_13;
	RMIOGroup[RMIO_3].GPIOPinState=GPIO_PIN_RESET;
	
	RMIOGroup[RMIO_5].GPIOGroup=GPIOB;
	RMIOGroup[RMIO_5].GPIOPin=GPIO_PIN_15;
	RMIOGroup[RMIO_5].GPIOPinState=GPIO_PIN_RESET;
	
	
	RMIOGroup[RMIO_7].GPIOGroup=GPIOB;
	RMIOGroup[RMIO_7].GPIOPin=GPIO_PIN_14;
	RMIOGroup[RMIO_7].GPIOPinState=GPIO_PIN_RESET;

}
void RMIO_Update(){
	for(int i=0;i<4;i++){
		HAL_GPIO_WritePin(RMIOGroup[i].GPIOGroup,RMIOGroup[i].GPIOPin,RMIOGroup[i].GPIOPinState);
	}
}

void GPIO_Init(void){

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

void RMIO_WritePin(GPIO_RMIO * RMPIN,GPIO_PinState state){
	HAL_GPIO_WritePin(RMPIN->GPIOGroup,RMPIN->GPIOPin,state);
}