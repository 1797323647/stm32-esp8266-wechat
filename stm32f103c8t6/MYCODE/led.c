#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// LED �˿����ú���ʵ��
void LED_Configuration(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;

    // 1. ʹ�� LED �˿�ʱ�� (GPIOC)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 2. ���� LED ���ŵ�ģʽ���ٶ�
    GPIO_InitStructure.GPIO_Pin = LED_PIN;            //  LED �������ӵ����� (PC13)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //  �������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //  GPIO �ٶ�Ϊ 50MHz
    GPIO_Init(LED_PORT, &GPIO_InitStructure);          //  �����趨������ʼ�� GPIOC

    // ��ѡ����ʼ״̬Ϩ�� LED
    GPIO_ResetBits(LED_PORT, LED_PIN);              //  ��ʼ״̬���� LED ��Ϩ��
}
