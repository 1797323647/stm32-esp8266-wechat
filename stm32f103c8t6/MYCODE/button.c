// button.c
#include "button.h"
#include "delay.h" // ���� delay.h ͷ�ļ���ʹ�� delay_ms ����

// ��ʼ������ GPIO
void BUTTON_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ�� GPIOA ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // ��������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// ��ȡ����״̬ (�����ȥ��)
uint8_t BUTTON_Read(void)
{
    static uint8_t button_state = 0; // ��̬������¼����״̬
    uint8_t current_state;

    current_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1); // ��ȡ PA1 ���ŵ�ƽ

    if (current_state == 0) // �������� (PA1 Ϊ�͵�ƽ)
    {
        delay_ms(20); // ��ʱȥ����
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) // �ٴ�ȷ�ϰ����Ƿ���
        {
            if (button_state == 0) // ���֮ǰ����״̬Ϊδ����
            {
                button_state = 1;  // ���°���״̬Ϊ����
                return 1;          // ���ذ������±�־
            }
        }
    }
    else // �����ɿ� (PA1 Ϊ�ߵ�ƽ)
    {
        button_state = 0; // ���°���״̬Ϊδ����
    }
    return 0; // ���ذ���δ���±�־
}
