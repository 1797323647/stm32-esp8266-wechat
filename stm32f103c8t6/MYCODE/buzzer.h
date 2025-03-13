#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "stm32f10x.h"

// ---  定义蜂鸣器控制引脚 ---
#define BUZZER_PORT   GPIOB      // 蜂鸣器控制端口，这里假设使用 GPIOB
#define BUZZER_PIN    GPIO_Pin_1  // 蜂鸣器控制引脚，这里假设使用 PB15

// ---  定义蜂鸣器控制宏 ---
#define BUZZER_ON   GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)   // 蜂鸣器 ON (高电平触发)
#define BUZZER_OFF  GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN)  // 蜂鸣器 OFF (低电平)
#define BUZZER_TOGGLE()  BUZZER_PORT->ODR ^= BUZZER_PIN  // 蜂鸣器状态翻转

// ---  函数声明 ---
void BUZZER_Init(void); // 蜂鸣器初始化函数

#endif /* _BUZZER_H_ */
