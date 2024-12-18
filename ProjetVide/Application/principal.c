#include "../Drivers/GPIO.h"
#include "../Drivers/TIMER.h"
#include "../Drivers/ADC.h"
#include "../Drivers/UART.h"

#include "stm32f10x.h"

/*

III - General-purpose and alternate-function I/Os (GPIOs and AFIOs)

1) ODR: Port output data register
Bit 0 ODR0: Port output data

*/

void Callback(void){
		GPIOA->ODR ^= GPIO_ODR_ODR0;  // Le bit ODR0 est invers�
	}

int Rapport_Cyclique;
uint16_t Valeur_ADC;
	
int main ( void )
{
	/*
	RCC->APB2ENR |= (0x01 << 2) | (0x01 << 3) | (0x01 << 4) ;
	// Quelle sera la valeur du registre (16 bits) APB2ENR apr�s l�ex�cution de cette instruction ?
	// En consid�rant que tous les bits du registre sont � 0, on aura : 0000 0000 0001 1100
	// Dans le cas g�n�ral : xxxx xxxx xxx1 11xx
	
	GPIOA->CRL &= ~(0xF<<4) ;
	GPIOA->CRL |= (0x2<<4) ;
	*/
	
	/* INTERRUPTION */
	
	/*
	N-1 dans ARR
	M-1 dans PSC
	Periode_Timer = N * Periode_Compteur
	Periode_Compteur = M * Periode_PSC
	Donc Periode_Timer = N * M * Periode_PSC

	On a Frequence_PSC = 72e6 Hz
	Donc Periode_PSC = 1/(72e6) s
	
	On veut Periode_Timer = 500 ms
	Donc N*M = 36e6
	Une solution possible : (N;M) = (36000;1000)
	*/
	
	MyTimer_Struct_TypeDef IT_500ms;
	IT_500ms.ARR = 36000-1;
	IT_500ms.PSC = 1000-1;
	
	IT_500ms.Timer = TIM4;
	MyTimer_Base_Init(&IT_500ms);
	MyTimer_Base_Start(TIM4);
	MyTimer_ActiveIT(TIM4,2,Callback);
		
	/* ADC */
	
	ourGPIO_struct GPIO_ADC;
	GPIO_ADC.GPIO = GPIOB;
	GPIO_ADC.GPIO_pin = 0; // Broche PB0
	GPIO_ADC.GPIO_conf = in_Analog;
	ourGPIO_Init(&GPIO_ADC);
		
	ourADC_struct mon_ADC;
	mon_ADC.ADC = ADC1;
	mon_ADC.ADC_channel = 8;
	mon_ADC.ADC_conf = c28cycles5;
	ourADC_Init(&mon_ADC); // ADC 1 Channel 8 -> Broche PB0

	/* PWM */
	
	/*
	N-1 dans ARR
	M-1 dans PSC
	R dans CCR
	Periode_PWM = N * Periode_Compteur
	Periode_Compteur = M * Periode_PSC
	Donc Periode_PWM = N * M * Periode_PSC

	On a Frequence_PSC = 72e6 Hz
	Donc Periode_PSC = 1/(72e6) s
	
	On veut Frequence_PWM = 50 kHz
	Donc Periode_PWM = 20 us
	Donc N*M = 1440
	Une solution possible : (N;M) = (1440;1)
	
	Alpha = R/N
	On a Alpha = Consigne (exemple 0.1)
	Donc R = Consigne*10 (exemple 1)
	*/
	
	ourGPIO_struct GPIO_PWM_50k;
	GPIO_PWM_50k.GPIO = GPIOA;
	GPIO_PWM_50k.GPIO_pin = 1; // Broche PA1
	GPIO_PWM_50k.GPIO_conf = altOut_Ppull;
	ourGPIO_Init(&GPIO_PWM_50k);
	
	MyTimer_Struct_TypeDef Timer_PWM_50k;
	Timer_PWM_50k.ARR = 1440-1;
	Timer_PWM_50k.PSC = 1-1;
	
	Timer_PWM_50k.Timer = TIM2;
	MyTimer_Base_Init(&Timer_PWM_50k);
	MyTimer_Base_Start(TIM2);
	MyTimer_PWM_Init_Channel(TIM2,2); // Timer 2 Channel 2 -> Broche PA1
	
	/*/*/
		
	ourGPIO_struct GPIO_PWM_50k_complementaire;
	GPIO_PWM_50k_complementaire.GPIO = GPIOA;
	GPIO_PWM_50k_complementaire.GPIO_pin = 6; // Broche PA6
	GPIO_PWM_50k_complementaire.GPIO_conf = altOut_Ppull;
	ourGPIO_Init(&GPIO_PWM_50k_complementaire);
	
	MyTimer_Struct_TypeDef Timer_PWM_50k_complementaire;
	Timer_PWM_50k_complementaire.ARR = 1440-1;
	Timer_PWM_50k_complementaire.PSC = 1-1;
	
	Timer_PWM_50k_complementaire.Timer = TIM3;
	MyTimer_Base_Init(&Timer_PWM_50k_complementaire);
	MyTimer_Base_Start(TIM3);
	MyTimer_PWM_Init_Channel(TIM3,1); // Timer 3 Channel 1 -> Broche PA6
	
	while(1){
		ourADC_Start(&mon_ADC);
		Valeur_ADC = ourADC_Read(&mon_ADC);
		if(0 <= Valeur_ADC && Valeur_ADC <= 81) Rapport_Cyclique = 2;
		if(82 <= Valeur_ADC && Valeur_ADC <= 4013) Rapport_Cyclique = 100*Valeur_ADC/4095;
		if(4014 <= Valeur_ADC && Valeur_ADC <= 4095) Rapport_Cyclique = 98;
		MyTimer_PWM_Set_Rapport_Cyclique(TIM2,2,Rapport_Cyclique);
		MyTimer_PWM_Set_Rapport_Cyclique(TIM3,1,100-Rapport_Cyclique);
	}
	
	/* UART */
	/*
	MyUART_Init(USART1,9600,72000000);
	MyUART_ActiveIT(USART1,Callback);
	MyUART_Transmit(USART1,'t');
	*/
	
}
