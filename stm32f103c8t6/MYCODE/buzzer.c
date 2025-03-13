#include "buzzer.h"

// ---  蜂鸣器初始化函数实现 ---
void BUZZER_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能 GPIOB 端口时钟 (根据 BUZZER_PORT 宏定义修改)

    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;         // 配置蜂鸣器控制引脚 (根据 BUZZER_PIN 宏定义修改)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO 速度配置
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);      // 初始化 GPIO

    BUZZER_OFF; // 初始化时关闭蜂鸣器
}
