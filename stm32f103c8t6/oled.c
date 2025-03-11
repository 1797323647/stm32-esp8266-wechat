//#include "oled.h"
//#include "stm32f10x.h"
//#include "delay.h"
//#include "codetab.h"
//#include <stdio.h>
//#include <string.h>

////��ʼ��Ӳ��IIC����
//void I2C_Configuration(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;
//		I2C_InitTypeDef I2C_InitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		
//		//PB6����SCL PB7����SDA
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

////��OLED�Ĵ�����ַдһ��byte������
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

////дָ��
//void WriteCmd(unsigned char I2C_Command)
//{
//		I2C_WriteByte(0x00,I2C_Command);
//}

////д����
//void WriteData(unsigned char I2C_Data)
//{
//		I2C_WriteByte(0x40,I2C_Data);
//}

////���ҳ�ʼ������
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
//		WriteCmd(0xff); //����?���̡�?�� 0x00~0xff
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

////���ù����ʼ���꣨x,y��
//void OLED_SetPos(unsigned char x,unsigned char y)
//{
//		WriteCmd(0xb0+y);
//		WriteCmd( (x & 0xf0) >> 4 | 0x10 );
//		WriteCmd( (x & 0x0f) | 0x01 );
//}

////���������Ļ
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

////����
//void OLED_CLS(void)
//{
//		OLED_Fill(0x00);
//}

////��OLED�������л���
//void OLED_ON(void)
//{
//		WriteCmd(0xAF);
//		WriteCmd(0x8D);
//		WriteCmd(0x14);
//}

////��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
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

//// ��ʾ���֣���Χ -9999 �� 9999
////void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
////{
////	unsigned char str[6] = {0}; // �㹻��� -9999 �� 9999 ���ַ������������źͽ�����
////	sprintf((char*)str, "%d", number); // ������ת��Ϊ�ַ���
////	OLED_ShowStr(x, y, str, TextSize); // ������ʾ�ַ���������ʾ�����ַ���
////}
//// ��ʾ���֣���Χ -9999 �� 9999��ʵ�ָ���д�룬�������ݲ���
//void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
//{
//    unsigned char str[6] = {0}; // �㹻��� -9999 �� 9999 ���ַ������������źͽ�����
//    sprintf((char*)str, "%d", number); // ������ת��Ϊ�ַ���

//    unsigned char num_width; // �����ַ����Ŀ�� (����)
//    if (TextSize == 1) {
//        num_width = 6 * strlen((char*)str); // 6x8 ���壬ÿ���ַ���� 6 ����
//    } else { // TextSize == 2
//        num_width = 8 * strlen((char*)str); // 8x16 ���壬ÿ���ַ���� 8 ����
//    }

//    // 1. ���������ʾ�����ÿո񸲸�
//    unsigned char space_str[7] = "      "; // 6 ���ո������㹻�Ŀռ串����� 5 λ���ּӷ���
//    OLED_ShowStr(x, y, space_str, TextSize); // �ÿո��ַ�������ԭ��������

//    // 2. д���µ�����
//    OLED_ShowStr(x, y, str, TextSize); // ������ʾ�ַ���������ʾ�����ַ���
//}

#include "oled.h"
#include "stm32f10x.h"
#include "delay.h"
#include "codetab.h"
#include <string.h>
#include <stdio.h>

// �������IIC����
#define OLED_SCL_PIN    GPIO_Pin_6
#define OLED_SDA_PIN    GPIO_Pin_7
#define OLED_GPIO       GPIOB
#define OLED_RCC        RCC_APB2Periph_GPIOB

// ���IIC�����궨��
#define OLED_SCL_HIGH()  GPIO_SetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SCL_LOW()   GPIO_ResetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SDA_HIGH()  GPIO_SetBits(OLED_GPIO, OLED_SDA_PIN)
#define OLED_SDA_LOW()   GPIO_ResetBits(OLED_GPIO, OLED_SDA_PIN)
#define OLED_SDA_READ()  GPIO_ReadInputDataBit(OLED_GPIO, OLED_SDA_PIN)

// ���IIC��ʱ���������Ը���ʵ���������
void OLED_IIC_Delay(void)
{
    uint8_t i = 10; // ���Ը���ʵ���ٶ��������
    while (i--);
}

// ��ʼ�����IIC�ӿ�
void I2C_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE); 
    
    // ����SCL��SDA����Ϊ�������
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    // ��ʼ��״̬��SCL��SDAΪ�ߵ�ƽ
    OLED_SCL_HIGH();
    OLED_SDA_HIGH();
}

// ���IIC��ʼ�ź�
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

// ���IICֹͣ�ź�
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

// �ȴ�Ӧ���ź�
uint8_t OLED_IIC_WaitAck(void)
{
    uint8_t ack;
    
    // ����SDAΪ����
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    OLED_SDA_HIGH();
    OLED_IIC_Delay();
    OLED_SCL_HIGH();
    OLED_IIC_Delay();
    
    // ��ȡSDA����״̬
    ack = OLED_SDA_READ();
    OLED_SCL_LOW();
    OLED_IIC_Delay();
    
    // �ָ�SDAΪ���
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
    
    return ack;
}

// ����Ӧ���ź�
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

// ������Ӧ���ź�
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

// ����һ���ֽ�
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

// ��OLED�Ĵ���д������
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    OLED_IIC_Start();
    OLED_IIC_SendByte(OLED_ADDRESS);  // д��ַ
    OLED_IIC_WaitAck();
    OLED_IIC_SendByte(addr);  // �Ĵ�����ַ
    OLED_IIC_WaitAck();
    OLED_IIC_SendByte(data);  // ����
    OLED_IIC_WaitAck();
    OLED_IIC_Stop();
}

// д����
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00, I2C_Command);
}

// д����
void WriteData(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40, I2C_Data);
}

// ���ҳ�ʼ������
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
    WriteCmd(0xff); //���ȵ��� 0x00~0xff
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

// ���ù����ʼ���꣨x,y��
void OLED_SetPos(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0 + y);
    WriteCmd((x & 0xf0) >> 4 | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

// ���������Ļ
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

// ����
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

// ��OLED�������л���
void OLED_ON(void)
{
    WriteCmd(0xAF);
    WriteCmd(0x8D);
    WriteCmd(0x14);
}

// ��OLED���� -- ����״̬��,OLED���Ĳ���10uA
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
// ��ʾ���֣���Χ -9999 �� 9999��ʵ�ָ���д�룬�������ݲ���
void OLED_ShowNumber(unsigned char x, unsigned char y, int number, unsigned char TextSize)
{
    unsigned char str[6] = {0}; // �㹻��� -9999 �� 9999 ���ַ������������źͽ�����
    sprintf((char*)str, "%d", number); // ������ת��Ϊ�ַ���

    unsigned char num_width; // �����ַ����Ŀ�� (����)
    if (TextSize == 1) {
        num_width = 6 * strlen((char*)str); // 6x8 ���壬ÿ���ַ���� 6 ����
    } else { // TextSize == 2
        num_width = 8 * strlen((char*)str); // 8x16 ���壬ÿ���ַ���� 8 ����
    }

    // 1. ���������ʾ�����ÿո񸲸�
    unsigned char space_str[7] = "      "; // 6 ���ո������㹻�Ŀռ串����� 5 λ���ּӷ���
    OLED_ShowStr(x, y, space_str, TextSize); // �ÿո��ַ�������ԭ��������

    // 2. д���µ�����
    OLED_ShowStr(x, y, str, TextSize); // ������ʾ�ַ���������ʾ�����ַ���
}
