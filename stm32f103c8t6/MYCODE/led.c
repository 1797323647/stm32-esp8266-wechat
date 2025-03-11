#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// LED 端口配置函数实现
void LED_Configuration(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;

    // 1. 使能 LED 端口时钟 (GPIOC)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 2. 配置 LED 引脚的模式和速度
    GPIO_InitStructure.GPIO_Pin = LED_PIN;            //  LED 灯所连接的引脚 (PC13)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //  推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //  GPIO 速度为 50MHz
    GPIO_Init(LED_PORT, &GPIO_InitStructure);          //  根据设定参数初始化 GPIOC

    // 可选：初始状态熄灭 LED
    GPIO_ResetBits(LED_PORT, LED_PIN);              //  初始状态设置 LED 灯熄灭
}
