#include "key.h"

uint8_t ucKey_val,ucKey_down,ucKey_up,ucKey_old;
uint8_t KEY_DOWN=0,KEY_UP=0;
__IO uint32_t Key_uwTick=0;

uint8_t Key_Scan(void){
	uint8_t keyVal =0;
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){
		keyVal = 1;
	}
	
	return keyVal;
}

void Key_Proc(void){
	if(uwTick-Key_uwTick < 50){
		return;
	}
	ucKey_val =Key_Scan();
	ucKey_down =ucKey_val & (ucKey_old ^ ucKey_val);
	ucKey_up =~ucKey_val & (ucKey_old ^ ucKey_val);
	ucKey_old =ucKey_val;
	//////////////////////////////////////////////////////
	if(ucKey_down == 1){
		KEY_DOWN=1;
	}
	if(ucKey_up == 1){
		KEY_UP=1;
	}
}