#ifndef _DELAY_H
#define _DELAY_H
/**************ͷ�ļ�����****************************/
#include "misc.h"
#include "stm32f10x.h"

/***************ȫ�ֱ������궨��***************************/



/***************��������***************************/
void Delay(int x);//ѭ����ʱ
void Tick_Delay_Config(void);
void delay_ms(u16 ms);
void delay_us(u32 us);
void delay_ms_By_Loop(int num);
#endif
