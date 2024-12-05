#include "ADC.h"

void ourADC_Init(ourADC_struct * ADCStructPtr){
	if(ADCStructPtr->ADC == ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	if(ADCStructPtr->ADC == ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

	char channel = ADCStructPtr->ADC_channel;
	char conf = ADCStructPtr->ADC_conf;
	
	// 1. Division de PCLK2 (72MHz Max) par 6
	// "The ADC input clock is generated from the PCLK2 clock divided by a prescaler and it must not exceed 14 MHz"
	RCC->CFGR &= ~(RCC_CFGR_ADCPRE);
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
	//RCC->CFGR &= ~(0x3 << 14); // 0x3 = 0b11
	//RCC->CFGR |= (0x2 << 14); // 0x2 = 0b10 : PCLK2 divided by 6
	
	// 2. Permission donnée à l'ADC de démarrer la conversion analogique-numérique
	ADCStructPtr->ADC->CR2 |= ADC_CR2_ADON;
	
	// 3. Configuration du temps d'échantillonnage (sample time) sur un canal donné
	// channel 0 à 9 : registre SMPR2
	if(channel <= 9) {
		ADCStructPtr->ADC->SMPR2 &= ~(0x07 << 3 * channel); // 0x7 = 0b111
		ADCStructPtr->ADC->SMPR2 |= (conf << 3 * channel);
	}
	// channel 10 à 17 : registre SMPR1
	else{
		ADCStructPtr->ADC->SMPR1 &= ~(0x07 << 3 * (channel - 10)); // 0x7 = 0b111
		ADCStructPtr->ADC->SMPR1 |= (conf << 3 * (channel - 10));
	}
	
	// 4. Configuration du canal sur le registre SQR3
	ADCStructPtr->ADC->SQR3 = channel; // Configuration sur les bits SQ1 : 1st conversion in regular sequence
	//ADCStructPtr->ADC->SQR3 &= ~(0x1F << 10); // 0x1F = 0b11111
	//ADCStructPtr->ADC->SQR3 |= (channel << 10); // Configuration sur les bits SQ3 : 3rd conversion in regular sequence
	
	// 5. Configuration de l'ADC en mode non-continu
	// Mode continu : l'ADC échantillonne le signal (fait des conversions) de manière constante, sans interruption
	ADCStructPtr->ADC->CR2 &= ~ADC_CR2_CONT;
	// ADCStructPtr->ADC->CR2 &= ~(0x01 << 1);
	
	// 6. Activation de la calibration de l'ADC
	ADCStructPtr->ADC->CR2 |= ADC_CR2_CAL;
	//ADCStructPtr->ADC->CR2 &= ~(0x01 << 2);
	//ADCStructPtr->ADC->CR2 |= (0x01 << 2);
	while (ADCStructPtr->ADC->CR2 & ADC_CR2_CAL);
	//while (ADCStructPtr->ADC->CR2 & (0x01 << 2));
	
	// 7. Activation du déclenchement externe (de la conversion en cas d'événement externe) pour les canaux réguliers
	ADCStructPtr->ADC->CR2 |= ADC_CR2_EXTTRIG;
	
	// 8. Démarrage de la conversion d'un groupe de canaux réguliers
	ADCStructPtr->ADC->CR2 |= ADC_CR2_EXTSEL;
	//ADCStructPtr->ADC->CR2 &= ~(0x07 << 17);
	//ADCStructPtr->ADC->CR2 |= (0x07 << 17);
}

void ourADC_Start(ourADC_struct * ADCStructPtr){
	ADCStructPtr->ADC->CR2 |= ADC_CR2_SWSTART; // Commencement de la conversion analogique-numérique
	while (!(ADCStructPtr->ADC->SR & ADC_SR_EOC)) {} // Attente de fin de conversion : exécution tant que le bit EOC n'est pas actif (que la conversion n'est pas terminée)
}
