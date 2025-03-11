// button.c
#include "button.h"
#include "delay.h" // 包含 delay.h 头文件，使用 delay_ms 函数

// 初始化按键 GPIO
void BUTTON_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能 GPIOA 时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 读取按键状态 (带软件去抖)
uint8_t BUTTON_Read(void)
{
    static uint8_t button_state = 0; // 静态变量记录按键状态
    uint8_t current_state;

    current_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1); // 读取 PA1 引脚电平

    if (current_state == 0) // 按键按下 (PA1 为低电平)
    {
        delay_ms(20); // 延时去抖动
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) // 再次确认按键是否按下
        {
            if (button_state == 0) // 如果之前按键状态为未按下
            {
                button_state = 1;  // 更新按键状态为按下
                return 1;          // 返回按键按下标志
            }
        }
    }
    else // 按键松开 (PA1 为高电平)
    {
        button_state = 0; // 更新按键状态为未按下
    }
    return 0; // 返回按键未按下标志
}
