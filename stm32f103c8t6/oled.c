//#include "oled.h"
//#include "stm32f10x.h"
//#include "delay.h"
//#include "codetab.h"
//#include <stdio.h>
//#include <string.h>

////初始化硬件IIC引脚
//void I2C_Configuration(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;
//		I2C_InitTypeDef I2C_InitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		
//		//PB6——SCL PB7——SDA
//		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
//		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		I2C_DeInit(I2C1);
//		I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
//		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//		I2C_InitStructure.I2C_ClockSpeed          = 400000;
//		I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
//		I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
//		I2C_InitStructure.I2C_OwnAddress1         = 0x30;
//		
//		I2C_Init(I2C1, &I2C_InitStructure);
//		I2C_Cmd(I2C1, ENABLE);
//}

////向OLED寄存器地址写一个byte的数据
//void I2C_WriteByte(uint8_t addr,uint8_t data)
//{
//		while( I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) );
//		I2C_GenerateSTART(I2C1, ENABLE);
//		
//		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) );
//		I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);
//		
//		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) );
//		I2C_SendData(I2C1, addr);
//		
//		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) );
//		I2C_SendData(I2C1, data);
//		
//		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) );
//		I2C_GenerateSTOP(I2C1, ENABLE);
//}

////写指令
//void WriteCmd(unsigned char I2C_Command)
//{
//		I2C_WriteByte(0x00,I2C_Command);
//}

////写数据
//void WriteData(unsigned char I2C_Data)
//{
//		I2C_WriteByte(0x40,I2C_Data);
//}

////厂家初始化代码
//void OLED_Init(void)
//{
//		delay_ms(100);
//		WriteCmd(0xAE); //display off
//		WriteCmd(0x20);	//Set Memory Addressing Mode	
//		WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
//		WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
//		WriteCmd(0xc8);	//Set COM Output Scan Direction
//		WriteCmd(0x00); //---set low column address
//		WriteCmd(0x10); //---set high column address
//		WriteCmd(0x40); //--set start line address
//		WriteCmd(0x81); //--set contrast control register
//		WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
//		WriteCmd(0xa1); //--set segment re-map 0 to 127
//		WriteCmd(0xa6); //--set normal display
//		WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
//		WriteCmd(0x3F); //
//		WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
//		WriteCmd(0xd3); //-set display offset
//		WriteCmd(0x00); //-not offset
//		WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
//		WriteCmd(0xf0); //--set divide ratio
//		WriteCmd(0xd9); //--set pre-charge period
//		WriteCmd(0x22); //
//		WriteCmd(0xda); //--set com pins hardware configuration
//		WriteCmd(0x12);
//		WriteCmd(0xdb); //--set vcomh
//		WriteCmd(0x20); //0x20,0.77xVcc
//		WriteCmd(0x8d); //--set DC-DC enable
//		WriteCmd(0x14); //
//		WriteCmd(0xaf); //--turn on oled panel

//}

////设置光标起始坐标（x,y）
//void OLED_SetPos(unsigned char x,unsigned char y)
//{
//		WriteCmd(0xb0+y);
//		WriteCmd( (x & 0xf0) >> 4 | 0x10 );
//		WriteCmd( (x & 0x0f) | 0x01 );
//}

////填充整个屏幕
//void OLED_Fill(unsigned char Fill_Data)
//{
//		unsigned char m,n;
//		
//		for(m=0;m<8;m++)
//		{
//				WriteCmd(0xb0+m);
//				WriteCmd(0x00);
//				WriteCmd(0x10);
//				
//				for(n=0;n<128;n++)
//				{
//						WriteData(Fill_Data);
//				}
//		}
//}

////清屏
//void OLED_CLS(void)
//{
//		OLED_Fill(0x00);
//}

////将OLED从休眠中唤醒
//void OLED_ON(void)
//{
//		WriteCmd(0xAF);
//		WriteCmd(0x8D);
//		WriteCmd(0x14);
//}

////让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//void OLED_OFF(void)
//{
//		WriteCmd(0xAE);
//		WriteCmd(0x8D);
//		WriteCmd(0x10);
//}

//void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize)
//{
//		unsigned char c = 0,i = 0,j = 0;
//		
//		switch(TextSize)
//		{
//			case 1:
//			{
//					while(ch[j] != '\0')
//					{
//							c = ch[j] - 32;
//							if(x>126)
//							{
//									x = 0;
//									y++;
//							}
//							
//							OLED_SetPos(x,y);
//							
//							for(i=0;i<6;i++)
//							{
//									WriteData(F6x8[c][i]);
//							}
//							x+=6;
//							j++;
//					}
//			}
//			break;
//			
//			case 2:
//			{
//					while(ch[j] != '\0')
//					{
//							c = ch[j] - 32;
//							
//							if(x>120)
//							{
//									x = 0;
//									y++;
//							}
//							
//							OLED_SetPos(x,y);
//							
//							for(i=0;i<8;i++)
//							{
//									WriteData(F8X16[c*16+i]);	
//							}
//							
//							OLED_SetPos(x,y+1);
//							
//							for(i=0;i<8;i++)
//							{
//									WriteData(F8X16[c*16+i+8]);
//							}
//							x+=8;
//							j++;		
//					}
//			}
//			break;
//		}
//}

//void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N)
//{
//		unsigned char i = 0;
//		unsigned char addr = 32*N;
//		
//		OLED_SetPos(x,y);
//		
//		for(i=0;i<16;i++)
//		{
//				WriteData(F16X16[addr]);
//				addr += 1;
//		}
//		
//		OLED_SetPos(x,y+1);
//		
//		for(i=0;i<16;i++)
//		{
//				WriteData(F16X16[addr]);
//				addr += 1;
//		}
//}

//void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
//{
//		unsigned char x,y;
//		unsigned int j = 0;
//		
//		if(y1 % 8 == 0)
//		{
//				y = y1 / 8;
//		}
//		else
//		{
//				y = y1 / 8+1;
//		}
//		
//		for(y=y0;y<y1;y++)
//		{
//				OLED_SetPos(x0,y);

//				for(x=x0;x<x1;x++)
//				{
//						WriteData(BMP1[j++]);
//				}
//		}
//}

//// 显示数字，范围 -9999 到 9999
////void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
////{
////	unsigned char str[6] = {0}; // 足够存放 -9999 到 9999 的字符串，包括负号和结束符
////	sprintf((char*)str, "%d", number); // 将整数转换为字符串
////	OLED_ShowStr(x, y, str, TextSize); // 调用显示字符串函数显示数字字符串
////}
//// 显示数字，范围 -9999 到 9999，实现覆盖写入，避免数据残留
//void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
//{
//    unsigned char str[6] = {0}; // 足够存放 -9999 到 9999 的字符串，包括负号和结束符
//    sprintf((char*)str, "%d", number); // 将整数转换为字符串

//    unsigned char num_width; // 数字字符串的宽度 (像素)
//    if (TextSize == 1) {
//        num_width = 6 * strlen((char*)str); // 6x8 字体，每个字符宽度 6 像素
//    } else { // TextSize == 2
//        num_width = 8 * strlen((char*)str); // 8x16 字体，每个字符宽度 8 像素
//    }

//    // 1. 清空数字显示区域：用空格覆盖
//    unsigned char space_str[7] = "      "; // 6 个空格，留出足够的空间覆盖最大 5 位数字加符号
//    OLED_ShowStr(x, y, space_str, TextSize); // 用空格字符串覆盖原数字区域

//    // 2. 写入新的数字
//    OLED_ShowStr(x, y, str, TextSize); // 调用显示字符串函数显示数字字符串
//}

#include "oled.h"
#include "stm32f10x.h"
#include "delay.h"
#include "codetab.h"
#include <string.h>
#include <stdio.h>

// 定义软件IIC引脚
#define OLED_SCL_PIN    GPIO_Pin_6
#define OLED_SDA_PIN    GPIO_Pin_7
#define OLED_GPIO       GPIOB
#define OLED_RCC        RCC_APB2Periph_GPIOB

// 软件IIC操作宏定义
#define OLED_SCL_HIGH()  GPIO_SetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SCL_LOW()   GPIO_ResetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SDA_HIGH()  GPIO_SetBits(OLED_GPIO, OLED_SDA_PIN)
#define OLED_SDA_LOW()   GPIO_ResetBits(OLED_GPIO, OLED_SDA_PIN)
#define OLED_SDA_READ()  GPIO_ReadInputDataBit(OLED_GPIO, OLED_SDA_PIN)

// 软件IIC延时函数，可以根据实际情况调整
void OLED_IIC_Delay(void)
{
    uint8_t i = 10; // 可以根据实际速度需求调整
    while (i--);
}

// 初始化软件IIC接口
void I2C_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE); 
    
    // 配置SCL和SDA引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    // 初始化状态，SCL和SDA为高电平
    OLED_SCL_HIGH();
    OLED_SDA_HIGH();
}

// 软件IIC开始信号
void OLED_IIC_Start(void)
{
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    OLED_SDA_LOW();
    OLED_IIC_Delay();
    OLED_SCL_LOW();
    OLED_IIC_Delay();
}

// 软件IIC停止信号
void OLED_IIC_Stop(void)
{
    OLED_SCL_LOW();
    OLED_SDA_LOW();
    OLED_IIC_Delay();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    OLED_SDA_HIGH();
    OLED_IIC_Delay();
}

// 等待应答信号
uint8_t OLED_IIC_WaitAck(void)
{
    uint8_t ack;
    
    // 配置SDA为输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    OLED_SDA_HIGH();
    OLED_IIC_Delay();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    
    // 读取SDA引脚状态
    ack = OLED_SDA_READ();
    OLED_SCL_LOW();
    OLED_IIC_Delay();
    
    // 恢复SDA为输出
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    return ack;
}

// 发送应答信号
void OLED_IIC_Ack(void)
{
    OLED_SCL_LOW();
    OLED_SDA_LOW();
    OLED_IIC_Delay();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    OLED_SCL_LOW();
    OLED_IIC_Delay();
}

// 不发送应答信号
void OLED_IIC_NAck(void)
{
    OLED_SCL_LOW();
    OLED_SDA_HIGH();
    OLED_IIC_Delay();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    OLED_SCL_LOW();
    OLED_IIC_Delay();
}

// 发送一个字节
void OLED_IIC_SendByte(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++)
    {
        OLED_SCL_LOW();
        OLED_IIC_Delay();
        
        if (data & 0x80)
            OLED_SDA_HIGH();
        else
            OLED_SDA_LOW();
        
        data <<= 1;
        OLED_IIC_Delay();
        OLED_SCL_HIGH();
        OLED_IIC_Delay();
    }
    
    OLED_SCL_LOW();
    OLED_IIC_Delay();
}

// 向OLED寄存器写入数据
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    OLED_IIC_Start();
    OLED_IIC_SendByte(OLED_ADDRESS);  // 写地址
    OLED_IIC_WaitAck();
    OLED_IIC_SendByte(addr);  // 寄存器地址
    OLED_IIC_WaitAck();
    OLED_IIC_SendByte(data);  // 数据
    OLED_IIC_WaitAck();
    OLED_IIC_Stop();
}

// 写命令
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00, I2C_Command);
}

// 写数据
void WriteData(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40, I2C_Data);
}

// 厂家初始化代码
void OLED_Init(void)
{
    delay_ms(100);
    WriteCmd(0xAE); //display off
    WriteCmd(0x20); //Set Memory Addressing Mode    
    WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); //Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //亮度调节 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
}

// 设置光标起始坐标（x,y）
void OLED_SetPos(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0 + y);
    WriteCmd((x & 0xf0) >> 4 | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

// 填充整个屏幕
void OLED_Fill(unsigned char Fill_Data)
{
    unsigned char m, n;
    
    for (m = 0; m < 8; m++)
    {
        WriteCmd(0xb0 + m);
        WriteCmd(0x00);
        WriteCmd(0x10);
        
        for (n = 0; n < 128; n++)
        {
            WriteData(Fill_Data);
        }
    }
}

// 清屏
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

// 将OLED从休眠中唤醒
void OLED_ON(void)
{
    WriteCmd(0xAF);
    WriteCmd(0x8D);
    WriteCmd(0x14);
}

// 让OLED休眠 -- 休眠状态下,OLED功耗不到10uA
void OLED_OFF(void)
{
    WriteCmd(0xAE);
    WriteCmd(0x8D);
    WriteCmd(0x10);
}

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0, i = 0, j = 0;
    
    switch (TextSize)
    {
        case 1:
            {
                while (ch[j] != '\0')
                {
                    c = ch[j] - 32;
                    if (x > 126)
                    {
                        x = 0;
                        y++;
                    }
                    
                    OLED_SetPos(x, y);
                    
                    for (i = 0; i < 6; i++)
                    {
                        WriteData(F6x8[c][i]);
                    }
                    x += 6;
                    j++;
                }
            }
            break;
        
        case 2:
            {
                while (ch[j] != '\0')
                {
                    c = ch[j] - 32;
                    
                    if (x > 120)
                    {
                        x = 0;
                        y++;
                    }
                    
                    OLED_SetPos(x, y);
                    
                    for (i = 0; i < 8; i++)
                    {
                        WriteData(F8X16[c * 16 + i]);    
                    }
                    
                    OLED_SetPos(x, y + 1);
                    
                    for (i = 0; i < 8; i++)
                    {
                        WriteData(F8X16[c * 16 + i + 8]);
                    }
                    x += 8;
                    j++;        
                }
            }
            break;
    }
}

void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char i = 0;
    unsigned char addr = 32 * N;
    
    OLED_SetPos(x, y);
    
    for (i = 0; i < 16; i++)
    {
        WriteData(F16X16[addr]);
        addr += 1;
    }
    
    OLED_SetPos(x, y + 1);
    
    for (i = 0; i < 16; i++)
    {
        WriteData(F16X16[addr]);
        addr += 1;
    }
}

void OLED_ShowBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned char x, y;
    unsigned int j = 0;
    
    if (y1 % 8 == 0)
    {
        y = y1 / 8;
    }
    else
    {
        y = y1 / 8 + 1;
    }
    
    for (y = y0; y < y1; y++)
    {
        OLED_SetPos(x0, y);

        for (x = x0; x < x1; x++)
        {
            WriteData(BMP1[j++]);
        }
    }
}
// 显示数字，范围 -9999 到 9999，实现覆盖写入，避免数据残留
void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
{
    unsigned char str[6] = {0}; // 足够存放 -9999 到 9999 的字符串，包括负号和结束符
    sprintf((char*)str, "%d", number); // 将整数转换为字符串

    unsigned char num_width; // 数字字符串的宽度 (像素)
    if (TextSize == 1) {
        num_width = 6 * strlen((char*)str); // 6x8 字体，每个字符宽度 6 像素
    } else { // TextSize == 2
        num_width = 8 * strlen((char*)str); // 8x16 字体，每个字符宽度 8 像素
    }

    // 1. 清空数字显示区域：用空格覆盖
    unsigned char space_str[7] = "      "; // 6 个空格，留出足够的空间覆盖最大 5 位数字加符号
    OLED_ShowStr(x, y, space_str, TextSize); // 用空格字符串覆盖原数字区域

    // 2. 写入新的数字
    OLED_ShowStr(x, y, str, TextSize); // 调用显示字符串函数显示数字字符串
}
