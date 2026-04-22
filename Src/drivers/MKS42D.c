#include "MKS42D.h"
#include "gpio.h"
#include "usart.h"
#include "can.h"
uint8_t ucCANTrans[20]="";
__IO uint32_t mks42d_uwTick=0;
int64_t mks42d_encoder[1]={0};

void MoveMKS42D(uint8_t num, uint16_t speed, uint8_t acc, int32_t absPulse)//speed->r/min//0x01 //3200step
{
	  if(absPulse > 8388607 || absPulse < -8388608) {
        if(absPulse > 8388607) absPulse = 8388607;
        if(absPulse < -8388608) absPulse = -8388608;
    }//
		if(absPulse > 7.62*STEPS_PER_REVOLUTION || absPulse < 0){//7.62*3200
			if(absPulse > 7.62*STEPS_PER_REVOLUTION) absPulse=7.62*STEPS_PER_REVOLUTION;
			if(absPulse < 0) absPulse=0;
		}
    // 提取24位有符号脉冲数（存储为32位，取低24位）

		
		uint32_t pulse = (uint32_t)(absPulse & 0x00FFFFFF); // 屏蔽高8位，保留低24位
		
    uint8_t speed_high = (speed >> 8) & 0xFF;
    uint8_t speed_low = speed & 0xFF;

    uint8_t pulse_byte4 = (pulse >> 16) & 0xFF;  // 最高字节
    uint8_t pulse_byte5 = (pulse >> 8) & 0xFF;
    uint8_t pulse_byte6 = pulse & 0xFF;           // 最低字节
    
    // 计算CRC校验和：地址 + 功能码 + 速度高 + 速度低 + 加速度 + 脉冲数3字节
    uint8_t crc = num;           // 地址
    crc += 0xFE;                // 功能码
    crc += speed_high;
    crc += speed_low;
    crc += acc;
    crc += pulse_byte4;
    crc += pulse_byte5;
    crc += pulse_byte6;
    // uint8_t加法自动取模256，得到低8位累加和
    
    // 填充发送数据数组（8字节数据场）
    ucCANTrans[0] = 0xFE;       // 功能码
    ucCANTrans[1] = speed_high; // 速度高字节
    ucCANTrans[2] = speed_low;  // 速度低字节
    ucCANTrans[3] = acc;        // 加速度
    ucCANTrans[4] = pulse_byte4; // 脉冲数最高字节
    ucCANTrans[5] = pulse_byte5; // 脉冲数次高字节
    ucCANTrans[6] = pulse_byte6; // 脉冲数最低字节
    ucCANTrans[7] = crc;        // CRC校验和
    
    // 配置CAN发送报文头
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = num;           // 标准标识符（电机地址）
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;      // 标准帧
    TxHeader.RTR = CAN_RTR_DATA;    // 数据帧
    TxHeader.DLC = 8;               // 数据长度8字节
    TxHeader.TransmitGlobalTime = DISABLE;
    
    // 发送CAN报文
    uint32_t mailbox;  // 发送邮箱号
    HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan2, &TxHeader, ucCANTrans, &mailbox);
		
		if(status != HAL_OK) {
			//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendFail",12,2);
    }
		else{
			//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendSucc",12,2);
		}
		
}

uint8_t ReadMKSEncoder(uint8_t num)//放在主循环
{
    // 发送查询指令，需要包含CRC校验和
    uint8_t tx_data[2];
    tx_data[0] = 0x31;  // 功能码：读取累加制编码器值
    
    // 计算CRC校验和：地址 + 功能码
    uint8_t crc = num + 0x31;
    tx_data[1] = crc;
    
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = num;           // 目标电机地址
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 2;               // 数据长度2字节，不是1字节
    TxHeader.TransmitGlobalTime = DISABLE;
    
    uint32_t mailbox;
    HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan2, &TxHeader, tx_data, &mailbox);
    
    if(status != HAL_OK) {
				//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendFail",12,2);
        return 0;  // 发送失败
    }
		else{
			//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendSucc",12,2);
		}
    
    return 1;
}

void MKS42D_Proc(){//放主循环0.1s一次
	if(uwTick-mks42d_uwTick < 100){
		return;
	}
	mks42d_uwTick=uwTick;
	
	ReadMKSEncoder(MKS42D_1);
}