#include "buzzer.h"

// ---  ��������ʼ������ʵ�� ---
void BUZZER_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ�� GPIOB �˿�ʱ�� (���� BUZZER_PORT �궨���޸�)

    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;         // ���÷������������� (���� BUZZER_PIN �궨���޸�)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO �ٶ�����
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);      // ��ʼ�� GPIO

    BUZZER_OFF; // ��ʼ��ʱ�رշ�����
}
