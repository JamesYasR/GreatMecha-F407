#include "MKS42D.h"
#include "gpio.h"
#include "usart.h"
#include "can.h"
#include "string.h"
#include "oled.h"
uint8_t ucCANTrans[20]="";
__IO uint32_t mks42d_uwTick=0;


MKS42D_INFO MKS42D1_INFO;

MKS42D_INFO MKS42D2_INFO;

int32_t MKS42d2_Dest=0;

void MKS42D_Init(){
	MKS42D1_INFO.axis=0;
	MKS42D1_INFO.speed=0;
	MKS42D2_INFO.axis=0;
	MKS42D2_INFO.speed=0;
}


void MoveMKS42D_absPluse(uint8_t num, uint16_t speed, uint8_t acc, int32_t absPulse)//speed->r/min//0x01 //3200step
{
		if(speed > 3000) {
			speed=3000;
    }
	  if(absPulse > 8388607 || absPulse < -8388608) {
        if(absPulse > 8388607) absPulse = 8388607;
        if(absPulse < -8388608) absPulse = -8388608;
    }//
		if(absPulse < -ALL_STROKE1*STEPS_PER_REVOLUTION || absPulse > 0){//7.62*3200
			if(absPulse < -ALL_STROKE1*STEPS_PER_REVOLUTION) absPulse=-ALL_STROKE1*STEPS_PER_REVOLUTION;
			if(absPulse > 0) absPulse=0;
		}
		
		if(absPulse < -ALL_STROKE2*STEPS_PER_REVOLUTION || absPulse > 0){//9.72*3200
			if(absPulse < -ALL_STROKE2*STEPS_PER_REVOLUTION) absPulse=-ALL_STROKE2*STEPS_PER_REVOLUTION;
			if(absPulse > 0) absPulse=0;
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
		memset(ucCANTrans,0,sizeof(ucCANTrans));
}

void MoveMKS42D_absAxis(uint8_t num, uint16_t speed, uint8_t acc,int32_t absAxis)//speed->r/min//0x01 //3200step
{
		if(speed > 3000) {
			speed=3000;
    }
		
	  if(absAxis > 8388607 || absAxis < -8388608) {
        if(absAxis > 8388607) absAxis = 8388607;
        if(absAxis < -8388608) absAxis = -8388608;
    }
		
		if(num==MKS42D_1){
			if(absAxis < -ALL_STROKE1*AXIS_PER_REVOLUTION || absAxis > 0){//7.55*3200
				if(absAxis < -ALL_STROKE1*AXIS_PER_REVOLUTION) absAxis=-ALL_STROKE1*AXIS_PER_REVOLUTION;
				if(absAxis > 0) absAxis=0;
			}
		}
		if(num==MKS42D_2){
			if(absAxis < -ALL_STROKE2*AXIS_PER_REVOLUTION || absAxis > 0){//7.55*3200
				if(absAxis < -ALL_STROKE2*AXIS_PER_REVOLUTION) absAxis=-ALL_STROKE2*AXIS_PER_REVOLUTION;
				if(absAxis > 0) absAxis=0;
			}
		}
    // 提取24位有符号脉冲数（存储为32位，取低24位）

		
		uint32_t axis = (uint32_t)(absAxis & 0x00FFFFFF); // 屏蔽高8位，保留低24位
		
    uint8_t speed_high = (speed >> 8) & 0xFF;
    uint8_t speed_low = speed & 0xFF;

    uint8_t axis_byte4 = (axis >> 16) & 0xFF;  // 最高字节
    uint8_t axis_byte5 = (axis >> 8) & 0xFF;
    uint8_t axis_byte6 = axis & 0xFF;           // 最低字节
    
    // 计算CRC校验和：地址 + 功能码 + 速度高 + 速度低 + 加速度 + 脉冲数3字节
    uint8_t crc = num;           // 地址
    crc += 0xF5;                // 功能码
    crc += speed_high;
    crc += speed_low;
    crc += acc;
    crc += axis_byte4;
    crc += axis_byte5;
    crc += axis_byte6;
    //自动溢出
    
    // 填充发送数据数组（8字节数据场）
    ucCANTrans[0] = 0xF5;       // 功能码
    ucCANTrans[1] = speed_high; // 速度高字节
    ucCANTrans[2] = speed_low;  // 速度低字节
    ucCANTrans[3] = acc;        // 加速度
    ucCANTrans[4] = axis_byte4; // 脉冲数最高字节
    ucCANTrans[5] = axis_byte5; // 脉冲数次高字节
    ucCANTrans[6] = axis_byte6; // 脉冲数最低字节
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
			temp++;
			//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendFail",12,2);
    }
		else{
			//HAL_UART_Transmit(&huart1,(uint8_t *)"Can2SendSucc",12,2);
		}
		memset(ucCANTrans,0,sizeof(ucCANTrans));
}

void SetMKS42D_Speed(uint8_t num, uint8_t dir,uint16_t speed, uint8_t acc)//F6 //dir=0->正转
{
		if(speed > 3000) {
			speed=3000;
    }
		if(dir!=1 && dir!=0){
			return;
		}
		uint8_t byte1=0xF6;
		uint8_t byte2;
		speed=speed & 0x0FFF;//只取低12位
		if(dir==0){
			byte2=0x00 + ((speed>>8) & 0x0F);
		}
		else if(dir==1){
			byte2=0x80 + ((speed>>8) & 0x0F);//1000 0000=0x80
		}
		uint8_t byte3=speed & 0xFF;
		uint8_t byte4=acc;
    
    // 计算CRC校验和：地址 + 功能码 + 速度高 + 速度低 + 加速度 + 脉冲数3字节
    uint8_t crc = num;           // 地址
    crc += byte1;                // 功能码
    crc += byte2; 
    crc += byte3; 
    crc += byte4; 
    //自动溢出
    
    // 填充发送数据数组（5字节数据场）
    ucCANTrans[0] = byte1;       // 功能码
    ucCANTrans[1] = byte2; // 速度高字节
    ucCANTrans[2] = byte3;  // 速度低字节
    ucCANTrans[3] = byte4;        // 加速度
    ucCANTrans[4] = crc;        // CRC校验和
    
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = num;           // 标准标识符（电机地址）
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;      // 标准帧
    TxHeader.RTR = CAN_RTR_DATA;    // 数据帧
    TxHeader.DLC = 5;               // 数据长度8字节
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
		memset(ucCANTrans,0,sizeof(ucCANTrans));
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

uint8_t ReadMKSSpeed(uint8_t num)//放在主循环
{
    // 发送查询指令，需要包含CRC校验和
    uint8_t tx_data[2];
    tx_data[0] = 0x32;  // 功能码：读取累加制编码器值
    
    // 计算CRC校验和：地址 + 功能码
    uint8_t crc = num + 0x32;
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

/**
 * @brief 发送读取累计脉冲数指令
 * @param num 目标电机地址
 * @return 发送成功返回1，失败返回0
 */
uint8_t ReadMKSPulse(uint8_t num)
{
    // 发送查询指令，需要包含CRC校验和
    uint8_t tx_data[2];
    tx_data[0] = 0x33;  // 功能码：读取输入累计脉冲数
    
    // 计算CRC校验和：地址 + 功能码
    uint8_t crc = num + 0x33;
    tx_data[1] = crc;
    
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = num;           // 目标电机地址
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 2;               // 数据长度2字节
    TxHeader.TransmitGlobalTime = DISABLE;
    
    uint32_t mailbox;
    HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan2, &TxHeader, tx_data, &mailbox);
    
    if(status != HAL_OK) {
        // 可选的调试信息
        // HAL_UART_Transmit(&huart1, (uint8_t *)"Can2SendPulseFail", 16, 2);
        return 0;  // 发送失败
    }
    else {
        // 可选的调试信息
        // HAL_UART_Transmit(&huart1, (uint8_t *)"Can2SendPulseSucc", 16, 2);
    }
    
    return 1;
}

void MKS42D_Proc(){//放主循环0.1s一次
	if(uwTick-mks42d_uwTick < 59){
		return;
	}
	mks42d_uwTick=uwTick;
	
	//ReadMKSEncoder(MKS42D_1);
	//ReadMKSSpeed(MKS42D_1);
	ReadMKSPulse(MKS42D_1);
	
	
	//ReadMKSSpeed(MKS42D_2);
	ReadMKSPulse(MKS42D_2);
}