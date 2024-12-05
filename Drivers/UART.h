#ifndef UART_H
#define UART_H

#include "stm32f10x.h"

void MyUART_Init(USART_TypeDef * UART, unsigned int debit);
// Débit en Bauds

void MyUART_Send(USART_TypeDef * UART, char data);
char MyUART_Read(USART_TypeDef * UART);
void MyUART_ActiveIT(USART_TypeDef * UART, void ( * IT_function ) (void));

#endif
