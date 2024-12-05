#include "stm32f10x.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/TIMER.h"
#include "../Drivers/ADC.h"
#include "../Service/DEFINE.h"

/*

III - General-purpose and alternate-function I/Os (GPIOs and AFIOs)

1) ODR: Port output data register
Bit 0 ODR0: Port output data

*/

void Callback(void){
		GPIOA->ODR ^= GPIO_ODR_ODR0;  // Le bit ODR0 est inversé
	}

int main ( void )
{
	/*
	RCC->APB2ENR |= (0x01 << 2) | (0x01 << 3) | (0x01 << 4) ;
	// Quelle sera la valeur du registre (16 bits) APB2ENR après l’exécution de cette instruction ?
	// En considérant que tous les bits du registre sont à 0, on aura : 0000 0000 0001 1100
	// Dans le cas général : xxxx xxxx xxx1 11xx
	
	GPIOA->CRL &= ~(0xF<<4) ;
	GPIOA->CRL |= (0x2<<4) ;
	*/

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
	Une solution possible : (N;M) = (1000;36000)
	*/
	
	
	MyTimer_Struct_TypeDef Timer500ms_IT;
	Timer500ms_IT.ARR = 1000-1;
	Timer500ms_IT.PSC = 36000-1;
	
	Timer500ms_IT.Timer = TIM1;
	MyTimer_Base_Init(&Timer500ms_IT);
	MyTimer_Base_Start(TIM1);
	MyTimer_ActiveIT(TIM1,2,Callback);
	
	/*
	Timer500.Timer = TIM2;
	MyTimer_Base_Init(&Timer500);
	MyTimer_Base_Start(TIM2);
	MyTimer_ActiveIT(TIM2,2,Callback);

	Timer500.Timer = TIM3;
	MyTimer_Base_Init(&Timer500);
	MyTimer_Base_Start(TIM3);
	MyTimer_ActiveIT(TIM3,2,Callback);

	Timer500.Timer = TIM4;
	MyTimer_Base_Init(&Timer500);
	MyTimer_Base_Start(TIM4);
	MyTimer_ActiveIT(TIM4,2,Callback);
	*/
	
	/*
	N-1 dans ARR
	M-1 dans PSC
	R dans CCR
	Periode_PWM = N * Periode_Compteur
	Periode_Compteur = M * Periode_PSC
	Donc Periode_PWM = N * M * Periode_PSC

	On a Frequence_PSC = 72e6 Hz
	Donc Periode_PSC = 1/(72e6) s
	
	On veut Frequence_PWM = 100kHz
	Donc Periode_PWM = 10 us
	Donc N*M = 720
	Une solution possible : (N;M) = (10;72)
	
	Alpha = R/N
	On a Alpha = 0,2
	Donc R = 2	
	*/
	
	MyTimer_Struct_TypeDef Timer10us_PWM;
	Timer10us_PWM.ARR = 10-1;
	Timer10us_PWM.PSC = 72-1;
	
	Timer10us_PWM.Timer = TIM2;
	MyTimer_Base_Init(&Timer10us_PWM);
	MyTimer_Base_Start(TIM2);
	MyTimer_PWM_Init_Channel(TIM2,2);
	MyTimer_PWM_Set_Rapport_Cyclique(TIM2,2,20);
	
	ourGPIO_struct mon_GPIO;
	mon_GPIO.GPIO = GPIO_Timer_PWM;
	mon_GPIO.GPIO_pin = Pin_Timer_PWM;
	mon_GPIO.GPIO_conf = altOut_Ppull;
	ourGPIO_Init(&mon_GPIO);
	//ourGPIO_Set(&mon_GPIO,Pin_Timer_PWM);
	//int test = ourGPIO_Read(&mon_GPIO,Pin_Timer_PWM);
	
	ourADC_struct mon_ADC;
	mon_ADC.ADC = ADC_Test;
	mon_ADC.ADC_channel = Channel_ADC_Test;
	mon_ADC.ADC_conf = Conf_ADC_Test;
	ourADC_Init(&mon_ADC);
	ourADC_Start(&mon_ADC);
	
	while (1)
	{
	}
}
