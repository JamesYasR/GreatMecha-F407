#include "SoleValve.h"
#include "Servo.h"
#include "oled.h"

SoleValve sv[3];
__IO uint32_t sv_uwTick=0;
void solevalve_init(){
	sv[0].GPIOGroup=GPIOE;
	sv[0].GPIOPin=GPIO_PIN_9;
	
	sv[1].GPIOGroup=GPIOE;
	sv[1].GPIOPin=GPIO_PIN_11;
	
	sv[2].GPIOGroup=GPIOE;
	sv[2].GPIOPin=GPIO_PIN_13;
}

void valve_open(SoleValve * sv){
	HAL_GPIO_WritePin(sv->GPIOGroup,sv->GPIOPin,GPIO_PIN_SET);
}


void valve_close(SoleValve * sv){
	HAL_GPIO_WritePin(sv->GPIOGroup,sv->GPIOPin,GPIO_PIN_RESET);
}



