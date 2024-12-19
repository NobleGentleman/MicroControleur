#ifndef UART_H
#define UART_H

#include "stm32f10x.h"

void MyUART_Init(USART_TypeDef * UART, unsigned int debit, int frequence_horloge);
// Débit (Baud Rate) : vitesse de transmission des données (en bits/symboles par seconde ou "Bauds")
// Fréquence de l'horloge de l'UART : vitesse de fonctionnement de l'UART, nombre de cycles par seconde (en Hz)
// Ici F = 72MHz

void MyUART_SendChar(USART_TypeDef * UART, char c);
void MyUART_SendString(USART_TypeDef * UART, char *str);
char MyUART_Read(USART_TypeDef * UART);
void MyUART_ActiveIT(USART_TypeDef * UART, void ( * IT_function ) (void));

#endif
