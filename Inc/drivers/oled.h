/**
 * ************************************************************************
 * 
 * @file OLED_IIC_Config.h
 * @author zxr
 * @brief IIC和OLED基础配置头文件
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr 
 * ************************************************************************
 */
#ifndef OLED_H
#define OLED_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define  OLED_ADDRESS 		0x78	//OLED地址  默认0x78

//OLED命令控制宏
#define  	LEFT 			0x27
#define  	RIGHT 			0x26
#define  	UP 			0X29
#define  	DOWM			0x2A
#define  	ON			0xA7
#define  	OFF			0xA6


#define    	SCREEN_PAGE_NUM		(8)     //屏幕页数
#define    	SCREEN_PAGEDATA_NUM	(128)   //每页的数据个数
#define		SCREEN_COLUMN		(128)   //列数
#define  	SCREEN_ROW		(64)    //行数
#define line0 0
#define line1 9
#define line2 18
#define line3 27
#define line4 36
#define line5 45
#define line6 54

void WriteCmd(unsigned char cmd);		//写命令
void WriteDat(unsigned char dat);		//写数据
void OLED_ON(void);				//开启OLED
void OLED_OFF(void);				//休眠OLED
void OLED_CLS(void);				//OLED清屏函数
void OLED_Init(void);				//OLED初始化函数
void OLED_RefreshRAM(void);			//更新数据缓冲区
void OLED_ClearRAM(void);			//清除数据缓冲区
void OLED_SetPixel(signed short int x, signed short int y, unsigned char set_pixel);	//设置坐标像素点数据
void OLED_DisplayMode(unsigned char mode);	//屏幕内容取反显示
void OLED_IntensityControl(unsigned char intensity);//屏幕亮度调节
void OLED_Shift(unsigned char shift_num);	//全屏内容偏移指定距离
void OLED_HorizontalShift(unsigned char start_page,unsigned char end_page,unsigned char direction);	//屏幕内容水平方向滚动播放


void OLED_ShowStr(signed short int x, signed short int y, unsigned char ch[], unsigned char TextSize);
//中文汉字显示函数
void OLED_ShowChinese(signed short int x, signed short int y, unsigned char* ch);
//BMP图片显示函数
void OLED_ShowBMP(signed short int x0,signed short int y0,signed short int L,signed short int H,const unsigned char BMP[]);

extern uint8_t oled_line0[21];
extern uint8_t oled_line1[21];
extern uint8_t oled_line2[21];
extern uint8_t oled_line3[21];
extern uint8_t oled_line4[21];
extern uint8_t oled_line5[21];
extern uint8_t oled_line6[21];
extern __IO uint32_t oled_uwTick;

void OLED_proc();
#endif  /*OLEDG_H*/

