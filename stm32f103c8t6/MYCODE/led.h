#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

// 定义 LED 连接的 GPIO 端口和引脚
#define LED_PORT      GPIOC
#define LED_PIN       GPIO_Pin_13

// LED 端口配置函数声明
void LED_Configuration(void);

// LED 控制宏定义
#define LEDOFF      GPIO_SetBits(LED_PORT, LED_PIN)     // 熄灭 LED
#define LEDON   GPIO_ResetBits(LED_PORT, LED_PIN)   // 点亮 LED
#define LED_TOGGLE  GPIO_WriteBit(LED_PORT, LED_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_PIN))) // LED 翻转

#endif /* _LED_H_ */
