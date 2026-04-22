#include "oled.h"
#include "oledfont.h"
#include "i2c.h"
#include "serial.h"
#include "it.h"
#include "Servo.h"
#include "rmYel.h"
unsigned char  ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN];//定义屏幕的存储空间
uint8_t oled_line_[21]="123456789012345678901";
uint8_t oled_line0[21]="                     ";
uint8_t oled_line1[21]="                     ";
uint8_t oled_line2[21]="                     ";
uint8_t oled_line3[21]="                     ";
uint8_t oled_line4[21]="                     ";
uint8_t oled_line5[21]="                     ";
uint8_t oled_line6[21]="                     ";

__IO uint32_t oled_uwTick=0;
/**
* @brief  向OLED寄存器地址写一个byte的数据
* @param  addr：寄存器地址
* @param  data：要写入的数据
* @retval 无
*/
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
	extern I2C_HandleTypeDef hi2c2;
	HAL_I2C_Mem_Write(&hi2c2, OLED_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

/**
 * ************************************************************************
 * @brief 写命令函数
 * @param[in] cmd  写入的命令
 * ************************************************************************
 */
void WriteCmd(unsigned char cmd)
{
	I2C_WriteByte(0x00, cmd);
}

/**
 * ************************************************************************
 * @brief 写数据函数
 * @param[in] dat  写入的数据
 * ************************************************************************
 */
void WriteDat(unsigned char dat)
{
	I2C_WriteByte(0x40, dat);
}

/**
 * ************************************************************************
 * @brief 开启OLED
 * ************************************************************************
 */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

/**
 * ************************************************************************
 * @brief 休眠OLED
 * ************************************************************************
 */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

/**
 * ************************************************************************
 * @brief OLED清屏函数
 * ************************************************************************
 */
void OLED_CLS(void)//清屏
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);	//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(0x00);
		}
	}
}

/**
 * ************************************************************************
 * @brief OLED初始化函数
 * ************************************************************************
 */
void OLED_Init(void)
{
	WriteCmd(0xAE); //显示关闭
	WriteCmd(0x20);	//设置内存寻址模式
	WriteCmd(0x10);	//00,水平寻址模式;01,垂直寻址模式;10,页寻址模式(复位);11,无效
	WriteCmd(0xb0);	//设置页寻址模式的页起始地址,0-7
	WriteCmd(0xc8);	//设置COM输出扫描方向
	WriteCmd(0x00); //-设置低列地址
	WriteCmd(0x10); //-设置高列地址
	WriteCmd(0x40); //-设置起始行地址
	WriteCmd(0x81); //设置对比度控制寄存器
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //设置段重新映射0到127
	WriteCmd(0xa6); //设置正常显示
	WriteCmd(0xa8); //设置复用比例(1到64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,输出遵循RAM内容;0xa5,输出忽略RAM内容
	WriteCmd(0xd3); //设置显示偏移
	WriteCmd(0x00); //不偏移
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //设置DC-DC使能
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}

/**
 * ************************************************************************
 * @brief 更新数据缓冲区
 * ************************************************************************
 */
void OLED_RefreshRAM(void)
{
	// 页寻址模式填充
	for(unsigned short int m = 0; m < SCREEN_ROW/8; m++)
	{
		WriteCmd(0xb0+m);		//设置页地址b0~b7
		WriteCmd(0x00);			//设置显示位置—列低地址00-0f
		WriteCmd(0x10);			//设置显示位置—列高地址10-1f
		for(unsigned short int n = 0; n < SCREEN_COLUMN; n++)
		{
			WriteDat(ScreenBuffer[m][n]);
		}
	} 
}

/**
 * ************************************************************************
 * @brief 清除数据缓冲区
 * ************************************************************************
 */
void OLED_ClearRAM(void)
{
	for(unsigned short int m = 0; m < SCREEN_ROW/8; m++)
	{
		for(unsigned short int n = 0; n < SCREEN_COLUMN; n++)
		{
			ScreenBuffer[m][n] = 0x00;
		}
	}
}

/**
 * ************************************************************************
 * @brief 设置坐标像素点数据
 * 
 * @param[in] x  			起始点横坐标(x:0~127）
 * @param[in] y  			起始点纵坐标(y:0~63）
 * @param[in] set_pixel  	该点的数据  SET_PIXEL = 1, RESET_PIXEL = 0
 * 
 * ************************************************************************
 */
void OLED_SetPixel(signed short int x, signed short int y, unsigned char set_pixel)
{ 
	if (x >= 0 && x < SCREEN_COLUMN && y >= 0 && y < SCREEN_ROW) {
		if(set_pixel){
				ScreenBuffer[y/8][x] |= (0x01 << (y%8));
		}  
		else{
				ScreenBuffer[y/8][x] &= ~(0x01 << (y%8));
		}
	}
}

/**
 * ************************************************************************
 * @brief 屏幕内容取反显示
 * 
 * @param[in] mode  开关
 * 					开	ON	0xA7	点亮全屏
 *  				关	OFF	0xA6	默认此模式，设置像素点亮
 * 
 * ************************************************************************
 */
void OLED_DisplayMode(unsigned char mode)
{
	WriteCmd(mode);
}

/**
 * ************************************************************************
 * @brief 屏幕亮度调节
 * 
 * @param[in] intensity  亮度大小（0~255），默认为0x7f
 * 
 * ************************************************************************
 */
void OLED_IntensityControl(unsigned char intensity)
{
	WriteCmd(0x81);
	WriteCmd(intensity);
}

/**
 * ************************************************************************
 * @brief 全屏内容偏移指定距离
 * 
 * @param[in] shift_num  偏移距离（0~63）
 * 
 * ************************************************************************
 */
void OLED_Shift(unsigned char shift_num)
{
	for(unsigned char i = 0; i < shift_num; i++)
		{
			WriteCmd(0xd3);//设置显示偏移，垂直向上偏移
			WriteCmd(i);//偏移量
			HAL_Delay(10);//延时时间
		}
}


/**
 * ************************************************************************
 * @brief 屏幕内容水平方向滚动播放
 * 
 * @param[in] start_page  	开始页数	(0~7)
 * @param[in] end_page  	结束页数	(0~7)
 * @param[in] direction  	滚动方向
 * 								左		LEFT	0x27
 * 								右		RIGHT	0x26
 * @note 在开始页数和结束页数之间的内容才会滚动播放，且写命令顺序不得改变
 * ************************************************************************
 */
void OLED_HorizontalShift(unsigned char start_page,unsigned char end_page,unsigned char direction)
{
	WriteCmd(0x2e);  //关闭滚动

	WriteCmd(direction);//设置滚动方向
	WriteCmd(0x00);//虚拟字节设置，默认为0x00
	WriteCmd(start_page);//设置开始页地址
	WriteCmd(0x05);//设置每个滚动步骤之间的时间间隔的帧频
	WriteCmd(end_page);//设置结束页地址
	WriteCmd(0x00);//虚拟字节设置，默认为0x00
	WriteCmd(0xff);//虚拟字节设置，默认为0xff

	WriteCmd(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

void OLED_ShowStr(signed short int x, signed short int y, unsigned char ch[], unsigned char TextSize)
{ 
	if (x >= 0 && x < SCREEN_COLUMN && y >= 0 && y < SCREEN_ROW) 
	{
		int32_t c = 0;
		unsigned char j = 0;
	
		switch(TextSize)
		{
			case 1:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//无效字符
						break;
					
					if(x >= 125 || (127-x < 6))//一行最大显示字符数：21字节显示,多出两列，不显示 || 剩余列小于6不能显示完整字符，换行显示
					{
						x = 0;
						y += 8;//换行显示
						if(63 - y < 8)	// 不足以显示一行时不显示
							break;
					}
					for(unsigned char m = 0; m < 6; m++)
					{
						for(unsigned char n = 0; n < 8; n++)
						{
							OLED_SetPixel(x+m, y+n, (F6x8[c][m] >> n) & 0x01);
						}
					}
					x += 6;
					j++;
				}
			}break;
			case 2:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//无效字符
						break;
					
					if(x >= 127 || (127-x < 8))//16字节显示 || 剩余列小于8不能显示完整字符，换行显示
					{
						x = 0;
						y += 16;//换行显示
						if(63 - y < 16)	// 不足以显示一行时不显示
							break;
					}
					for(unsigned char m = 0; m < 2; m++)
					{
						for(unsigned char n = 0; n < 8; n++)
						{
							for(unsigned char i = 0; i < 8; i++)
							{
								OLED_SetPixel(x+n, y+i+m*8, (F8X16[c][n+m*8] >> i) & 0x01);
							}
						}	
					}
					x += 8;
					j++;
				}
			}break;
		}
	}
	//OLED_RefreshRAM();
}




/**
 * ************************************************************************
 * @brief BMP图片显示函数
 * 
 * @param[in] x0  	起始点横坐标（0~127）
 * @param[in] y0  	起始点纵坐标（0~63）
 * @param[in] L  	BMP图片宽度
 * @param[in] H  	BMP图片高度
 * @param[in] BMP  	图片取模地址
 * 
 * @example OLED_ShowBMP(0,0,52,48,(unsigned char *)astronaut_0);
 * ************************************************************************
 */
void OLED_ShowBMP(signed short int x0,signed short int y0,signed short int L,signed short int H,const unsigned char BMP[])
{
	if (x0 >= 0 && x0 < SCREEN_COLUMN && x0+L <= SCREEN_ROW &&\
		y0 >= 0 && y0 < SCREEN_COLUMN && y0+H <= SCREEN_ROW) {
		
		unsigned char *p = (unsigned char *)BMP;
		for(signed short int y = y0; y < y0+H; y+=8)
		{
			for(signed short int x = x0; x < x0+L; x++)
			{
				for(signed short int i = 0; i < 8; i++)
				{
					OLED_SetPixel(x, y+i, ((*p) >> i) & 0x01);
				}
				p++;
			}
		}
	}

	OLED_RefreshRAM();
}

void OLED_proc(){
	if(uwTick-oled_uwTick < 250){
		return;
	}
	oled_uwTick=uwTick;
	
	sprintf((char *)oled_line0,"Point:%.4f,%.4f ",Point_Received[0],Point_Received[1]);//系统时间 TIM6时间
	
	sprintf((char *)oled_line1,"n:%.2f        ",(double)mks42d_encoder[0]/16384.0);//系统时间 TIM6时间
	sprintf((char *)oled_line4,"time:%d %d        ",uwTick/1000,TIM6_Tick/1000);//系统时间 TIM6时间
  sprintf((char *)oled_line5,"uartr:%d %d         ",uart_recNum[0],uart_recNum[1]);
	sprintf((char *)oled_line6,"debug:%d %d %d  ",error_recNum[0],error_recNum[1],error_serial[0]);//串口1 串口2 解析
	
	OLED_ShowStr(0,line0,oled_line0,1);
	OLED_ShowStr(0,line1,oled_line1,1);
	OLED_ShowStr(0,line2,oled_line2,1);
	OLED_ShowStr(0,line3,oled_line3,1);
	OLED_ShowStr(0,line4,oled_line4,1);
	OLED_ShowStr(0,line5,oled_line5,1);
	OLED_ShowStr(0,line6,oled_line6,1);
	
	OLED_RefreshRAM();
}
