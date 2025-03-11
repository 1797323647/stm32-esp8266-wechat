#include "uart.h"
#include "stm32f10x.h" 

// ���� UART2 ���ͺͽ��ջ����� (�����Ҫ�жϽ��գ�����Ҫ������)
#define UART2_TX_BUFFER_SIZE 128
#define UART2_RX_BUFFER_SIZE 128

uint8_t UART2_TxBuffer[UART2_TX_BUFFER_SIZE];
uint8_t UART2_RxBuffer[UART2_RX_BUFFER_SIZE];
uint16_t UART2_TxCounter = 0;
uint16_t UART2_RxCounter = 0;
volatile uint8_t UART2_DataAvailable = 0; // ����Ƿ���յ�����

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}

void uart_init(u32 bound){
//GPIO�˿�����
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;	 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��  
//USART1_TX   GPIOA.9
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9 
//USART1_RX	  GPIOA.10��ʼ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10 
//Usart1 NVIC ����
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//USART ��ʼ������
USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
USART_Init(USART1, &USART_InitStructure); //��ʼ������1
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

// ��ʼ�� UART2 (��׼��汾)
void UART2_Configuration(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. ʹ�� UART2 �� GPIOA ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ʹ�� UART2 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ�� GPIOA ʱ��

    // 2. ���� UART2 �� GPIO ����
    //    UART2_TX (PA2) ����Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //    UART2_RX (PA3) ����Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ���� UART2 ����
    USART_InitStructure.USART_BaudRate = baudrate;         // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // 8 λ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;        // 1 ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;            // ����żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;               // ʹ�ܷ��ͺͽ���ģʽ

    USART_Init(USART2, &USART_InitStructure);

    // 4. ʹ�� UART2 �����ж� (�����Ҫ��������)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ�ܽ��շǿ��ж� (ע���ж�ʹ�ܺ������)

    // 5. ���� NVIC �жϿ�����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;            // UART2 �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);

    // 6. ʹ�� UART2
    USART_Cmd(USART2, ENABLE);
}

// UART2 �жϷ����� (��������) - ��׼��汾
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { // �������жϱ�־ (ע���жϱ�־�������)
        // ���յ�����
        UART2_RxBuffer[UART2_RxCounter++] = USART_ReceiveData(USART2);
        if (UART2_RxCounter >= UART2_RX_BUFFER_SIZE) {
            UART2_RxCounter = 0; // ������������������ѭ������
        }
        UART2_DataAvailable = 1; // ��ǽ��յ�����
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ��������жϱ�־ (ע���жϱ�־�������)
    }
}

// ����һ���ֽ����� (��׼��汾)
void UART2_SendChar(uint8_t byte) {
    USART_SendData(USART2, (uint16_t)byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // �ȴ��������
}

// �����ַ��� (��׼��汾)
void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

// ����һ���ֽ����� (��������ʽ����׼��汾)
uint8_t UART2_ReceiveChar(void) {
    if (UART2_DataAvailable) {
        UART2_DataAvailable = 0; // ������ݿ��ñ�־
        return UART2_RxBuffer[UART2_RxCounter - 1]; // ���ؽ��յ������һ���ֽ�
    } else {
        return 0; // û�����ݷ��� 0
    }
}

// ��ȡ���յ����ַ��� (��׼��汾)
char* UART2_ReceiveString(char *buffer, uint16_t bufferSize) {
    uint16_t i = 0;
    char receivedChar;
    while (i < bufferSize - 1) {
        receivedChar = UART2_ReceiveChar();
        if (receivedChar != 0) { // ���յ���Ч����
            buffer[i++] = receivedChar;
            if (receivedChar == '\n') { // ���Ը���ʵ����������ַ���������
                break;
            }
        } else {
            // û�н��յ����ݣ�������ӳ�ʱ�����
            break;
        }
    }
    buffer[i] = '\0'; // �ַ�����β��
    return buffer;
}
