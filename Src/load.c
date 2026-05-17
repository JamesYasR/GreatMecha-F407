#include "load.h"
#include "MKS42D.h"
#include "SoleValve.h"
#include "tim.h"
#include "gpio.h"

#define STEP0 0
#define STEP1 1
#define STEP2 2
#define STEP3 3
#define STEP4 4
#define STEP5 5
#define STEP6 6

#define STEPS 10500
#define TIMES 1000

#define POS1 -1.5f
#define POS2 -1.4f

__IO uint32_t load_uwTick=0;
__IO uint32_t tray_uwTick=0;
__IO uint32_t lastload_uwTick=0;
__IO uint32_t lasttray_uwTick=0;
//0->17 怎么触发？ signal置1的时候
//然后根据0->17的状态 //0-> 9 九步前进 9->17->0 回到0状态

uint8_t load_state=STEP0;
uint8_t tray_state=0;
uint8_t tray_signal=0;
uint8_t tray_moving=0;

uint8_t CSD_FLAG=0;


void load_proc(){
	if(uwTick-load_uwTick < 100){
		return;
	}
	load_uwTick=uwTick;
	
	if(tray_state==6){
		if(MKS42D2_INFO.pulse <= POS2 *STEPS_PER_REVOLUTION){
			MoveMKS42D_absAxis(MKS42D_2,600,10,-1.5 * AXIS_PER_REVOLUTION);
		}
		return;
	}
	
	if(load_state==STEP0){
		valve_open(sv+0);
		MoveMKS42D_absAxis(MKS42D_2,600,10,-1.0 *ALL_STROKE2 * AXIS_PER_REVOLUTION);
		lastload_uwTick=uwTick;
		load_state=STEP1;
	}
	else if(load_state==STEP1){
		if(MKS42D2_INFO.pulse <= -0.98 *ALL_STROKE2 *STEPS_PER_REVOLUTION){
			lastload_uwTick=uwTick;
			load_state=STEP2;
		}
	}
	else if(load_state==STEP2){
		if(uwTick - lastload_uwTick > 100){
			//tray_signal=1;
			valve_close(sv+0);
			lastload_uwTick=uwTick;
			load_state=STEP3;
		}
	}

	else if(load_state==STEP3){
		MoveMKS42D_absAxis(MKS42D_2,600,10,-1.5 * AXIS_PER_REVOLUTION);
		lastload_uwTick=uwTick;
		load_state=STEP4;
	}
	
	else if(load_state==STEP4){
		if(MKS42D2_INFO.pulse >= -1.6 *STEPS_PER_REVOLUTION){
			lastload_uwTick=uwTick;
			load_state=STEP5;
		}
	}
	
	else if(load_state==STEP5){
		if(uwTick - lastload_uwTick > 100){
			tray_signal=1;
			valve_open(sv+0);
			lastload_uwTick=uwTick;
			load_state=STEP6;
		}
	}
	else if(load_state==STEP6){
		if(uwTick - lastload_uwTick > 100){
			lastload_uwTick=uwTick;
			load_state=STEP0;
		}
	}
	
}
	
void Tray_Move(){
	if(uwTick-tray_uwTick < 10){
		return;
	}
	tray_uwTick=uwTick;
	
	
	if(tray_state==6){
			RMIO_WritePin(RMIOGroup+RMIO_1,GPIO_PIN_RESET);
			if(tray_moving==0){
				lasttray_uwTick=uwTick;
				tray_moving=1;
				setPWM(&htim1,TIM_CHANNEL_4,200,0.0);
			}
			else if(tray_moving==1){
				if(uwTick - lasttray_uwTick >=10000){
					tray_moving=0;
					//tray_state=(tray_state+1)%11;
				}
			}
			return;
	}
	
	if(tray_signal==1 || tray_moving==1){
		tray_signal=0;
		if(tray_state>=0 && tray_state<=4){
			RMIO_WritePin(RMIOGroup+RMIO_1,GPIO_PIN_RESET);
			if(tray_moving==0){
				lasttray_uwTick=uwTick;
				tray_moving=1;
				setPWM(&htim1,TIM_CHANNEL_4,STEPS,0.2);
			}
			else if(tray_moving==1){
				if(uwTick - lasttray_uwTick >=TIMES){
					setPWM(&htim1,TIM_CHANNEL_4,200,0.0);
					tray_moving=0;
					tray_state=(tray_state+1)%7;
				}
			
			}
		}
		else if(tray_state>=5 && tray_state<6){
			RMIO_WritePin(RMIOGroup+RMIO_1,GPIO_PIN_SET);
			if(tray_moving==0){
				lasttray_uwTick=uwTick;
				tray_moving=1;
				setPWM(&htim1,TIM_CHANNEL_4,STEPS,0.2);
			}
			else if(tray_moving==1){
				if(uwTick - lasttray_uwTick >= 3000){
					setPWM(&htim1,TIM_CHANNEL_4,200,0.0);
					tray_moving=0;
					tray_state=(tray_state+1)%7;
				}
			
			}
		}
	}
	
	
}