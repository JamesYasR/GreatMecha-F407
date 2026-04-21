#include "main.h"
#include "init.h"
#include "stdio.h"
#include "string.h"
#include "it.h"
#include "key.h"
#include "MKS42D.h"
#include "serial.h"
#include "oled.h"
#include "i2c.h"
#include "Servo.h"
#include "rmYel.h"

int main(void)
{
	InitAll();
	HAL_Delay(1);
	
  while (1)
  {
		Key_Proc();
		Serial_proc();
		OLED_proc();
  }

}

