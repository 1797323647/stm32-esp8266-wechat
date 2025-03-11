#include "uart.h"
#include "stm32f10x.h" 

// 定义 UART2 发送和接收缓冲区 (如果需要中断接收，则需要缓冲区)
#define UART2_TX_BUFFER_SIZE 128
#define UART2_RX_BUFFER_SIZE 128

uint8_t UART2_TxBuffer[UART2_TX_BUFFER_SIZE];
uint8_t UART2_RxBuffer[UART2_RX_BUFFER_SIZE];
uint16_t UART2_TxCounter = 0;
uint16_t UART2_RxCounter = 0;
volatile uint8_t UART2_DataAvailable = 0; // 标记是否接收到数据

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}

void uart_init(u32 bound){
//GPIO端口设置
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;	 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟  
//USART1_TX   GPIOA.9
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9 
//USART1_RX	  GPIOA.10初始化
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10 
//Usart1 NVIC 配置
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//USART 初始化设置
USART_InitStructure.USART_BaudRate = bound;//串口波特率
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
USART_Init(USART1, &USART_InitStructure); //初始化串口1
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

// 初始化 UART2 (标准库版本)
void UART2_Configuration(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能 UART2 和 GPIOA 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 使能 UART2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能 GPIOA 时钟

    // 2. 配置 UART2 的 GPIO 引脚
    //    UART2_TX (PA2) 配置为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //    UART2_RX (PA3) 配置为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置 UART2 参数
    USART_InitStructure.USART_BaudRate = baudrate;         // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // 8 位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;        // 1 个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;            // 无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;               // 使能发送和接收模式

    USART_Init(USART2, &USART_InitStructure);

    // 4. 使能 UART2 接收中断 (如果需要接收数据)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能接收非空中断 (注意中断使能宏的名称)

    // 5. 配置 NVIC 中断控制器
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;            // UART2 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 使能中断通道
    NVIC_Init(&NVIC_InitStructure);

    // 6. 使能 UART2
    USART_Cmd(USART2, ENABLE);
}

// UART2 中断服务函数 (接收数据) - 标准库版本
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { // 检查接收中断标志 (注意中断标志宏的名称)
        // 接收到数据
        UART2_RxBuffer[UART2_RxCounter++] = USART_ReceiveData(USART2);
        if (UART2_RxCounter >= UART2_RX_BUFFER_SIZE) {
            UART2_RxCounter = 0; // 缓冲区溢出处理，这里简单循环覆盖
        }
        UART2_DataAvailable = 1; // 标记接收到数据
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除接收中断标志 (注意中断标志宏的名称)
    }
}

// 发送一个字节数据 (标准库版本)
void UART2_SendChar(uint8_t byte) {
    USART_SendData(USART2, (uint16_t)byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // 等待发送完成
}

// 发送字符串 (标准库版本)
void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

// 接收一个字节数据 (非阻塞方式，标准库版本)
uint8_t UART2_ReceiveChar(void) {
    if (UART2_DataAvailable) {
        UART2_DataAvailable = 0; // 清除数据可用标志
        return UART2_RxBuffer[UART2_RxCounter - 1]; // 返回接收到的最后一个字节
    } else {
        return 0; // 没有数据返回 0
    }
}

// 获取接收到的字符串 (标准库版本)
char* UART2_ReceiveString(char *buffer, uint16_t bufferSize) {
    uint16_t i = 0;
    char receivedChar;
    while (i < bufferSize - 1) {
        receivedChar = UART2_ReceiveChar();
        if (receivedChar != 0) { // 接收到有效数据
            buffer[i++] = receivedChar;
            if (receivedChar == '\n') { // 可以根据实际情况定义字符串结束符
                break;
            }
        } else {
            // 没有接收到数据，可以添加超时处理等
            break;
        }
    }
    buffer[i] = '\0'; // 字符串结尾符
    return buffer;
}
