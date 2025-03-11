#include "delay.h"

static u8 fac_us = 0;
static u16 fac_ms = 0;

void Delay(int x){
	for(int i = 0;i < x;i++){
		for(int j = 0;j<x;j++){
		
		}
	}
}

//滴答定时器初始化
void Tick_Delay_Config(){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock/8000000;//系统时钟的1/8
    fac_ms = (u16)fac_us * 1000;
}

//微秒
void delay_us(u32 us){
	u32 temp =0;
    SysTick->LOAD = us*fac_us;//时间加载值
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始计时
    do{
        temp = SysTick->CTRL;
    }while((temp &0x01) && !(temp &(1 << 16)));//等待延时时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器操作
    SysTick->VAL = 0x00;//清空计数器
}
//毫秒
void delay_ms(u16 ms){
	u32 temp =0;
    SysTick->LOAD = (u32)ms*fac_ms;//时间加载值
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始计时
    do{
        temp = SysTick->CTRL;
    }while((temp &0x01) && !(temp &(1 << 16)));//等待延时时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器操作
    SysTick->VAL = 0x00;//清空计数器
}

void delay_ms_By_Loop(int num){
	for(int i = 0; i < num; i++){
		delay_ms(1);
	}
}



