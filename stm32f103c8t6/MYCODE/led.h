#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

// ���� LED ���ӵ� GPIO �˿ں�����
#define LED_PORT      GPIOC
#define LED_PIN       GPIO_Pin_13

// LED �˿����ú�������
void LED_Configuration(void);

// LED ���ƺ궨��
#define LEDOFF      GPIO_SetBits(LED_PORT, LED_PIN)     // Ϩ�� LED
#define LEDON   GPIO_ResetBits(LED_PORT, LED_PIN)   // ���� LED
#define LED_TOGGLE  GPIO_WriteBit(LED_PORT, LED_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_PIN))) // LED ��ת

#endif /* _LED_H_ */
