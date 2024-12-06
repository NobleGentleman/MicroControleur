#ifndef UART_H
#define UART_H

#include "stm32f10x.h"

void MyUART_Init(USART_TypeDef * UART, unsigned int debit, int frequence_horloge);
// D�bit (Baud Rate) : vitesse de transmission des donn�es (en bits/symboles par seconde ou "Bauds")
// Fr�quence de l'horloge de l'UART : vitesse de fonctionnement de l'UART, nombre de cycles par seconde (en Hz)
// Ici F = 72MHz

void MyUART_Transmit(USART_TypeDef * UART, char data);
char MyUART_Receive(USART_TypeDef * UART);
void MyUART_ActiveIT(USART_TypeDef * UART, void ( * IT_function ) (void));

#endif
