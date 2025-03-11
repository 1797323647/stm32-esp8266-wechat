#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include <stdint.h>
void uart_init(uint32_t bound);
void UART2_Configuration(uint32_t baudrate);
void USART2_IRQHandler(void);
void UART2_SendChar(uint8_t byte);
void UART2_SendString(char *str);
uint8_t UART2_ReceiveChar(void);
char* UART2_ReceiveString(char *buffer, uint16_t bufferSize);
#endif
