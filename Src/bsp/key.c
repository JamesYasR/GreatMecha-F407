#include "key.h"
#include "usart.h"
#include "rmYel.h"
#include "SoleValve.h"
#include "can.h"
uint8_t ucKey_val,ucKey_down,ucKey_up,ucKey_old;
__IO uint32_t Key_uwTick=0;

uint8_t Key_Scan(void){
	uint8_t keyVal =0;
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){//000111000
		keyVal = 2;
	}
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9) == GPIO_PIN_RESET){
		keyVal = 4;
	}
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == GPIO_PIN_RESET){
		keyVal = 8;
	}
	return keyVal;
}

void Key_Proc(void){
	if(uwTick-Key_uwTick < 50){
		return;
	}
	Key_uwTick=uwTick;
	
	ucKey_val =Key_Scan();
	ucKey_down =ucKey_val & (ucKey_old ^ ucKey_val);//按钮按下就是下降沿
	ucKey_up =~ucKey_val & (ucKey_old ^ ucKey_val);
	ucKey_old =ucKey_val;
	if(ucKey_down==2){
			//HAL_UART_Transmit(&huart1,ucTrans1,strlen((char *)ucTrans1),2);
			temp++;
			Cut_Yel();
	}
	if(ucKey_up==4){
		//if(sv[0].State==IDLE){
			valve_open(sv+0);
		//}
	}
	if(ucKey_up==8){
		//if(sv[1].State==IDLE){
			valve_open(sv+1);
		//}
	}
}