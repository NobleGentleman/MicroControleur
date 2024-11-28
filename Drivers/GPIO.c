#include "GPIO.h"

void ourGPIO_Init( ourGPIO_struct * GPIOStructPtr){
	
	if(GPIOStructPtr->GPIO == GPIOA) RCC->APB2ENR |= (0x01 << 2);
	if(GPIOStructPtr->GPIO == GPIOB) RCC->APB2ENR |= (0x01 << 3);
	if(GPIOStructPtr->GPIO == GPIOC) RCC->APB2ENR |= (0x01 << 4);
	if(GPIOStructPtr->GPIO == GPIOD) RCC->APB2ENR |= (0x01 << 5);
	
	char pin = GPIOStructPtr->GPIO_pin;
	char conf = GPIOStructPtr->GPIO_conf;
	
	// pin 0 à 7 : registre CRL
	if(pin <= 7) {
		 	
		GPIOStructPtr->GPIO->CRL &= ~(0x0F << 4 * pin);
		// Configuration du registre CRL pour le pin correspondant (cf. Tableau 20)

		if(conf == 0x15) {
			conf = 0x08;
			//GPIOStructPtr->GPIO->ODR |= (0x01 << pin);
		}
		//else {
		//	GPIOStructPtr->GPIO->ODR |= (0x00 << pin);			
		//}
		// Configuration du registre ODR pour le pin correspondant (cf. Tableau 20)
		
		GPIOStructPtr->GPIO->CRL |= (conf <<  4 * pin);
		// Configuration du registre CRL pour le pin correspondant (cf. Tableau 20)
	}
	
	// pin 8 à 15 : registre CRH
	else {
		
		GPIOStructPtr->GPIO->CRH &= ~(0x0F << 4 * (pin - 8));
		// Configuration du registre CRL pour le pin correspondant (cf. Tableau 20)
		// Le registre CRH est utilisé pour les pin 8 à 15 donc, du point de vue du registre CRH, le pin 8 est à l'index 0, le pin 9 à l'index 1 etc.

		if(conf == 0x15) {
			conf = 0x08;
			//GPIOStructPtr->GPIO->ODR |= (0x01 << pin);
		}
		//else {
		//	GPIOStructPtr->GPIO->ODR |= (0x00 << pin);			
		//}
		// Configuration du registre ODR pour le pin correspondant (cf. Tableau 20)
		
		GPIOStructPtr->GPIO->CRH |= (conf <<  4 * (pin - 8));
		// Configuration du registre CRL pour le pin correspondant (cf. Tableau 20)
	}
}

int ourGPIO_Read( ourGPIO_struct * GPIOStructPtr, char GPIO_pin ){
	if( (GPIOStructPtr->GPIO->IDR & (0x01 << GPIO_pin)) == (0x01 << GPIO_pin)){
		// Partie gauche : IDR (Input Data Register) : registre d'entrée de données. Il sert à lire l'état des pins du GPIO.
		// Partie gauche (bis) : Opération '&' : Mise à 0 de tous les bits registre IDR sauf du bit correspondant à la pin spécifiée.
		// Partie droite : Masque binaire : Le nombre 1 est décalé à gauche de x rangs (avec x : numéro de la pin). Grâce à ce masque, seul le bit correspondant à la pin spécifiée est mis à 1.
		// Comparaison : La fonction retourne 1 si la pin spécifiée est à l'état haut, sinon elle retourne 0.
		return 1;
	} 
	return 0;
}

void ourGPIO_Set(ourGPIO_struct * GPIOStructPtr, char GPIO_pin){
	GPIOStructPtr->GPIO->BSRR |= (0x01 << GPIO_pin);
	// Solution alternative : GPIOStructPtr->GPIO->ODR |= (0x01 << GPIO_pin);
	
	// Configuration du registre ODR pour le pin donné (cf. Tableau 20)
	// Le registre BSRR (partie Set) fixe le bit du registre ODR correspondant correspondant au pin
}

void ourGPIO_Reset(ourGPIO_struct * GPIOStructPtr, char GPIO_pin){
	GPIOStructPtr->GPIO->BRR |= (0x01 << GPIO_pin);
	// Solution alternative n°1 : GPIOStructPtr->GPIO->BSRR |= (0x01 << GPIO_pin + 16);
	// Solution alternative n°2 : GPIOStructPtr->GPIO->ODR &= ~(0x01 << GPIO_pin);
	
	// Réinitialisation du bit du registre ODR correspondant au pin donné (cf. Tableau 20)
	// Les registres BRR et BSRR (partie Reset) réinitialise le bit du registre ODR correspondant au pin
}

void ourGPIO_Toggle(ourGPIO_struct * GPIOStructPtr, char GPIO_pin){
	GPIOStructPtr->GPIO->ODR ^= (0x01 << GPIO_pin);
	// Inversion du bit du registre ODR correspondant au pin donné (cf. Tableau 20)
}
