#include "load.h"
#include "MKS42D.h"
#include "SoleValve.h"
#define STEP0 0
#define STEP1 1
#define STEP2 2
#define STEP3 3
#define STEP4 4
#define STEP5 5
#define STEP6 6

__IO uint32_t load_uwTick=0;
__IO uint32_t lastload_uwTick=0;

uint8_t load_state=STEP0;

void test(){
	HAL_Delay(5000);
	MoveMKS42D_absAxis(MKS42D_2,600,10,-2.0 * AXIS_PER_REVOLUTION);
	HAL_Delay(5000);
	MoveMKS42D_absAxis(MKS42D_2,600,10,0.0 * AXIS_PER_REVOLUTION);
}

void load_proc(){
	if(uwTick-load_uwTick < 99){
		return;
	}
	load_uwTick=uwTick;
	
	if(load_state==STEP0){
		valve_open(sv+0);
		MoveMKS42D_absAxis(MKS42D_2,600,10,-1.0 *ALL_STROKE2 * AXIS_PER_REVOLUTION);
		lastload_uwTick=uwTick;
		load_state=STEP1;
	}
	if(load_state==STEP1){
		if(MKS42D2_INFO.pulse <= -0.98 *ALL_STROKE2 *STEPS_PER_REVOLUTION){
			lastload_uwTick=uwTick;
			load_state=STEP2;
		}
	}
	if(load_state==STEP2){
		if(uwTick - lastload_uwTick > 100){
			valve_close(sv+0);
			lastload_uwTick=uwTick;
			load_state=STEP3;
		}
	}

	if(load_state==STEP3){
		MoveMKS42D_absAxis(MKS42D_2,600,10,-1.0 * AXIS_PER_REVOLUTION);
		lastload_uwTick=uwTick;
		load_state=STEP4;
	}
	
	if(load_state==STEP4){
		if(MKS42D2_INFO.pulse >= -1.2 *STEPS_PER_REVOLUTION){
			lastload_uwTick=uwTick;
			load_state=STEP5;
		}
	}
	
	if(load_state==STEP5){
		if(uwTick - lastload_uwTick > 100){
			valve_open(sv+0);
			lastload_uwTick=uwTick;
			load_state=STEP6;
		}
	}
	if(load_state==STEP6){
		if(uwTick - lastload_uwTick > 100){
			lastload_uwTick=uwTick;
			load_state=STEP0;
		}
	}
	
}