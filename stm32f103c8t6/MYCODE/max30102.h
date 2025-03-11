#ifndef __MAX30102_H
#define __MAX30102_H
#include "sys.h"
#include "algorithm.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

//IO操作函数	 
#define IIC_SCL    PBout(7) //SCL
#define IIC_SDA    PBout(8) //SDA	 
#define READ_SDA   PBin(8)  //输入SDA 
#define MAX30102_INT PBin(9)

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define max30102_WR_address 0xAE

#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF
// MAX30102 相关变量声明
extern uint32_t aun_ir_buffer[500];
extern int32_t n_ir_buffer_length;
extern uint32_t aun_red_buffer[500];
extern int32_t n_sp02;
extern int8_t ch_spo2_valid;
extern int32_t n_heart_rate;
extern int8_t  ch_hr_valid;
extern uint8_t uch_dummy;
extern uint8_t dis_hr;
extern uint8_t dis_spo2;

// MAX30102 应用层初始化函数声明
void max30102_app_init(void);

// MAX30102 任务函数声明 (包含数据采集、处理和计算)
void max30102_task(void);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data);
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);  

void max30102_init(void);  
void max30102_reset(void);
u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data);
u8 max30102_Bus_Read(u8 Register_Address);
void max30102_FIFO_ReadWords(u8 Register_Address,u16  Word_Data[][2],u8 count);
void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data);
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);
#endif
















