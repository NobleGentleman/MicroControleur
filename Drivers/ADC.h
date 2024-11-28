#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

typedef struct{
	ADC_TypeDef * ADC;
	char ADC_channel; // Numéro de 0 à 17
	char ADC_conf;
} ourADC_struct;

#define c1cycle5 0x00
#define c7cycles5 0x01
#define c13cycles5 0x02
#define c28cycles5 0x03
#define c41cycles5 0x04
#define c55cycles5 0x05
#define c71cycles5 0x06
#define c239cycles5 0x07

void ourADC_Init(ourADC_struct * ADCStructPtr);
void ourADC_Start(ourADC_struct * ADCStructPtr);

#endif
