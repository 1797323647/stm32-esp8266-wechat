#ifndef _DELAY_H
#define _DELAY_H
/**************头文件包含****************************/
#include "misc.h"
#include "stm32f10x.h"

/***************全局变量、宏定义***************************/



/***************函数声明***************************/
void Delay(int x);//循环延时
void Tick_Delay_Config(void);
void delay_ms(u16 ms);
void delay_us(u32 us);
void delay_ms_By_Loop(int num);
#endif
