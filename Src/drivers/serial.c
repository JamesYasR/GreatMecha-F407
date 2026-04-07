#include "serial.h"
#include "math.h"
__IO uint32_t serial_uwTick=0;
float Point_Received[2];

void serial_proc(){
	if(uwTick-serial_uwTick < 10){
		return;
	}
	serial_uwTick=uwTick;
	if(usart6_tx_busy) return;
	if(IS_UART_RECEIVED[0]==RECEIVED){
		usart6_tx_busy = 1;
		IS_UART_RECEIVED[0]=UNRECEIVED;
		HAL_UART_Transmit_DMA(&huart1, ucRecei1, ucBuffer_len[0]);
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,ucRecei1,ucBuffSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
//		if(ucBuffer1[0] != STARTFLAG){
//			return;
//		}
//		else{
//			if(ucBuffer1[1] == SENDPOINTFLOATFLAG){
//				process_Point(ucBuffer1,ucBuffer_len[0]);
//				//sprintf((char *)ucTrans6,"point:%.2f,%.2f",Point_Received[0],Point_Received[1]);
//				//HAL_UART_Transmit(&huart6,ucTrans6,sizeof(ucTrans6),1000);
//			}
//			else{
//				return;
//			}
//		}
	}
	
}

float parse_float_string(uint8_t* str, uint16_t len) {
    float result = 0.0f;
    float fraction = 0.1f;
    uint8_t decimal_found = 0;
    
    for (uint16_t i = 0; i < len; i++) {
        if (str[i] == '.') {
            decimal_found = 1;
            continue;
        }
        
        if (decimal_found==0) {
            result = result * 10.0f + ascii2digit(str[i]);
        } else {
            result += ascii2digit(str[i]) * fraction;
            fraction *= 0.1f;
        }
    }
    return result;
}


void process_Point(uint8_t * str,uint16_t len) {
    if (len < 8) {  // 0x80 + 0xA1 + 最少1数字 + 0x82 + 最少1数字 + 0x81
        return;
    }
    
    if (str[0] != 0x80 || str[1] != 0xA1 || str[len-1] != 0x81) {
        return;
    }
    

    uint16_t split_index = 0;
    for (uint16_t i = 2; i < len - 1; i++) {
        if (str[i] == 0x82) {  // SPLITFLAG
            split_index = i;
            break;
        }
    }
    
    if (split_index == 0) {
        return; 
    }

    uint16_t x_len = split_index - 2;
    if (x_len == 0) return;
    
    uint16_t y_len = (len - 1) - (split_index + 1);
    if (y_len == 0) return;

    float number1 = parse_float_string(&str[2], x_len);
    float number2 = parse_float_string(&str[split_index + 1], y_len);
    
    Point_Received[0] = number1;
    Point_Received[1] = number2;

}



uint8_t ascii2digit(uint8_t ascii)
{
   if(ascii >= '0' && ascii <= '9') {
      return ascii - '0';
   }
	 
	 else{
			return 0xB3; 
	 }
}