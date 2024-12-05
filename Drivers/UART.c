#include "UART.h"
#include "GPIO.h"
void (*PTF1_UART) (void);
void (*PTF2_UART) (void);
void (*PTF3_UART) (void);

void MyUART_Init(USART_TypeDef * UART, unsigned int debit){
	// � faire.
}

void MyUART_Send(USART_TypeDef * UART, char data){
	while(!(UART->SR & USART_SR_TXE));
	// On attend que le registre de transmission de l'UART soit vide avant d'envoyer un nouveau caract�re
	// Cela permet de ne pas �craser les donn�es d�j� pr�sentes dans le registre de transmission
	// <-> Synchronisation de l'envoi de donn�es � travers l'UART
	
	UART->DR = data;
}

char MyUART_Read(USART_TypeDef * UART){
	return UART->DR;
}

void MyUART_ActiveIT(USART_TypeDef * UART, void ( * IT_function ) (void)){
	
	// Activation des interruptions pour deux �v�nements : lorsque des donn�es sont re�ues (RXNE) et lorsque des erreurs de parit� se produisent (PE)
	UART->CR1 |= (USART_CR1_RXNEIE | USART_CR1_PEIE);
	
	if (UART == USART1){
		PTF1_UART = IT_function;
		NVIC_EnableIRQ(USART1_IRQn); // Autoriser la prise en compte de l'interruption
	} else if (UART == USART2){
		PTF2_UART = IT_function;
		NVIC_EnableIRQ(USART2_IRQn); // Autoriser la prise en compte de l'interruption
	} else if (UART == USART3){
		PTF3_UART = IT_function;
		NVIC_EnableIRQ(USART3_IRQn); // Autoriser la prise en compte de l'interruption
	}
}

void USART1_IRQHandler(void){
	// V�rifie si le bit RXNE (Receive Not Empty) est set dans le registre de statut (SR)
	if(USART1->SR & USART_SR_RXNE) {
		USART1->SR &= ~USART_SR_RXNE;
		// Si RXNE est set, cela signifie qu'il y a des donn�es re�ues
		// R�initialisation du bit RXNE pour indiquer que la r�ception a �t� prise en compte
    (*PTF1_UART)(); 
	}
}

void USART2_IRQHandler(void){
	// Idem.
	if(USART2->SR & USART_SR_RXNE) {
		USART2->SR &= ~USART_SR_RXNE;
    (*PTF2_UART)(); 
	}
}

void USART3_IRQHandler(void){
	// Idem.
	if(USART3->SR & USART_SR_RXNE) {
		USART3->SR &= ~USART_SR_RXNE;
    (*PTF3_UART)();   
	}
}
