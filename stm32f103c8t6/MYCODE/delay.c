#include "delay.h"

static u8 fac_us = 0;
static u16 fac_ms = 0;

void Delay(int x){
	for(int i = 0;i < x;i++){
		for(int j = 0;j<x;j++){
		
		}
	}
}

//�δ�ʱ����ʼ��
void Tick_Delay_Config(){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock/8000000;//ϵͳʱ�ӵ�1/8
    fac_ms = (u16)fac_us * 1000;
}

//΢��
void delay_us(u32 us){
	u32 temp =0;
    SysTick->LOAD = us*fac_us;//ʱ�����ֵ
    SysTick->VAL = 0x00;//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//��ʼ��ʱ
    do{
        temp = SysTick->CTRL;
    }while((temp &0x01) && !(temp &(1 << 16)));//�ȴ���ʱʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�رռ���������
    SysTick->VAL = 0x00;//��ռ�����
}
//����
void delay_ms(u16 ms){
	u32 temp =0;
    SysTick->LOAD = (u32)ms*fac_ms;//ʱ�����ֵ
    SysTick->VAL = 0x00;//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//��ʼ��ʱ
    do{
        temp = SysTick->CTRL;
    }while((temp &0x01) && !(temp &(1 << 16)));//�ȴ���ʱʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�رռ���������
    SysTick->VAL = 0x00;//��ռ�����
}

void delay_ms_By_Loop(int num){
	for(int i = 0; i < num; i++){
		delay_ms(1);
	}
}



