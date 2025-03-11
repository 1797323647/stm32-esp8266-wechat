//#ifndef _oled_h_
//#define _oled_h_

//#include "stm32f10x.h"
//#define OLED_ADDRESS 0x78

//void I2C_Configuration(void);
//void I2C_WriteByte(uint8_t addr,uint8_t data);
//void WriteCmd(unsigned char I2C_Command);
//void WriteData(unsigned char I2C_Data);
//void OLED_Init(void);
//void OLED_SetPos(unsigned char x,unsigned char y);
//void OLED_Fill(unsigned char Fill_Data);
//void OLED_CLS(void);
//void OLED_ON(void);
//void OLED_OFF(void);
//void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);
//void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N);
//void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
//void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize);

//#endif

#ifndef _oled_h_
#define _oled_h_

#include "stm32f10x.h"
#define OLED_ADDRESS 0x78 // 0x3C<<1

// 软件IIC操作函数
void OLED_IIC_Delay(void);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
uint8_t OLED_IIC_WaitAck(void);
void OLED_IIC_Ack(void);
void OLED_IIC_NAck(void);
void OLED_IIC_SendByte(uint8_t data);

// OLED操作函数
void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteData(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char Fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_ShowBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize);

#endif
