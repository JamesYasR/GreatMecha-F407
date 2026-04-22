#include "serial.h"
#include "math.h"
#include "oled.h"
#include "MKS42D.h"
#include "Servo.h"
uint8_t TARGETUPDATE_FLAG=0;
__IO uint32_t serial_uwTick=0;
float Point_Received[2];
uint16_t error_serial[1]={0};

void Serial_proc(){
	if(uwTick-serial_uwTick < 10){
		return;
	}
	serial_uwTick=uwTick;
	
	if(IS_UART_RECEIVED[0]==RECEIVED){
		IS_UART_RECEIVED[0]=UNRECEIVED;
		if(ucBuffer1[1]==SENDPOINTFLOATFLAG){
			if(process_Point(ucBuffer1,ucBuffer_len[0])){
				TARGETUPDATE_FLAG=1;
				return;
			}
			else{
				error_serial[0]++;
				return;
			}
		}
		else if(ucBuffer1[1]==MKS42DTASKFLAG){//开始位 0x84 符号位 转速位 圈数位 结束位 5位
			if(ucBuffer1[2]==0x00){
				HAL_UART_Transmit(&huart1,(uint8_t *)"0",1,1);
				MKS42D_AddTask(MKS42D_0,1.0*ucBuffer1[3],0.03*ucBuffer1[4]);
			}
			else if(ucBuffer1[2]==0x01){
				HAL_UART_Transmit(&huart1,(uint8_t *)"1",1,1);
				MKS42D_AddTask(MKS42D_0,-1.0*ucBuffer1[3],0.03*ucBuffer1[4]);//7.71
			}

		}
		else if(ucBuffer1[1]==SERVOTASKFLAG){//开始位 0x85 符号位 转速位 时间位 结束位 5位
			if(ucBuffer1[2]==0x00){
				HAL_UART_Transmit(&huart1,(uint8_t *)"0",1,1);
				Set_Servo360Speed(servo360+0,0.1f*ucBuffer1[3],100*ucBuffer1[4]);//正转向上
			}
			else if(ucBuffer1[2]==0x01){
				HAL_UART_Transmit(&huart1,(uint8_t *)"1",1,1);
				Set_Servo360Speed(servo360+0,-0.1f*ucBuffer1[3],100*ucBuffer1[4]);//反转向下
			}
			else if(ucBuffer1[2]==0x02){
				HAL_UART_Transmit(&huart1,(uint8_t *)"2",1,1);
				Set_Servo180Angle(servo180+0,1.0f*ucBuffer1[3]);//直接设置角度0x00-0xB4
			}

		}

	}
	
}

uint8_t process_Point(uint8_t *str, uint16_t len)
{
    if (len < 12)
    {
        return 0;
    }
    
    for (uint16_t i = 0; i <= len - 12; i++)
    {
        if (str[i] == STARTFLAG)
        {
            
            if (str[i+1] == SENDPOINTFLOATFLAG)
            {
                
                if (str[i+6] == SPLITFLAG)
                {
                    if (i + 11 < len && str[i+11] == ENDFLAG)
                    {
                        float x, y;
                        memcpy(&x, &str[i+2], sizeof(float));
                        memcpy(&y, &str[i+7], sizeof(float));
                        
                        Point_Received[0] = x;
                        Point_Received[1] = y;
                        
                        
                        return 1;
                    }
                    else
                    {
                        error_serial[0]++;
                    }
                }
                else
                {
                    error_serial[0]++;
                }
            }
            else
            {
                error_serial[0]++;
            }
        }
    }
    
    error_serial[0]++;
    return 0;
}