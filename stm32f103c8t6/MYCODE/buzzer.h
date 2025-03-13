#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "stm32f10x.h"

// ---  ����������������� ---
#define BUZZER_PORT   GPIOB      // ���������ƶ˿ڣ��������ʹ�� GPIOB
#define BUZZER_PIN    GPIO_Pin_1  // �������������ţ��������ʹ�� PB15

// ---  ������������ƺ� ---
#define BUZZER_ON   GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)   // ������ ON (�ߵ�ƽ����)
#define BUZZER_OFF  GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN)  // ������ OFF (�͵�ƽ)
#define BUZZER_TOGGLE()  BUZZER_PORT->ODR ^= BUZZER_PIN  // ������״̬��ת

// ---  �������� ---
void BUZZER_Init(void); // ��������ʼ������

#endif /* _BUZZER_H_ */
