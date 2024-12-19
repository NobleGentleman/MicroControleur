#include "../Drivers/GPIO.h"
#include "../Drivers/TIMER.h"
#include "../Drivers/ADC.h"
#include "../Drivers/UART.h"

#include "stm32f10x.h"

#define NB_VALEURS_ADC 4096.0 // La résolution de l'ADC est de 12 bits
#define VREF 3.3 // Tension de référence (3,3V)

/*

III - General-purpose and alternate-function I/Os (GPIOs and AFIOs)

1) ODR: Port output data register
Bit 0 ODR0: Port output data

*/

int UART_Flag = 0; // Variable pour indiquer si l'UART doit envoyer des données
int Rapport_Cyclique;
uint16_t Valeur_ADC;


/* Fonction de callback pour l'interruption */
void Callback(void){
	GPIOA->ODR ^= GPIO_ODR_ODR0; // Le bit ODR0 est inversé
	UART_Flag = 1;
}

/* Conversion de la valeur analogique de l'ADC en volts :
Avec une résolution de 12 bits et une tension de référence de 3.3V,
la précision de cette conversion est de +/- 3.3/4096.0 V (environ égal à 0.8057 mV) */
float Valeur_ADC_en_Volts(uint16_t Val_ADC){
  return (Val_ADC/NB_VALEURS_ADC) * VREF;
}

int main(void){
	
	/* CODE EN LIEN AVEC LE TP1 (PAS EN RAPPORT AVEC LE PROJET) */
	
	/*
	RCC->APB2ENR |= (0x01 << 2) | (0x01 << 3) | (0x01 << 4) ;
	// Quelle sera la valeur du registre (16 bits) APB2ENR après l’exécution de cette instruction ?
	// En considérant que tous les bits du registre sont à 0, on aura : 0000 0000 0001 1100
	// Dans le cas général : xxxx xxxx xxx1 11xx
	
	GPIOA->CRL &= ~(0xF<<4) ;
	GPIOA->CRL |= (0x2<<4) ;
	*/
	
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
	
	/* PWM COMPLÉMENTAIRE */
		
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
	
	/* UART */
	
	ourGPIO_struct GPIO_UART;
	GPIO_UART.GPIO = GPIOB;
	GPIO_UART.GPIO_pin = 6; // Broche PB6
	GPIO_UART.GPIO_conf = altOut_Ppull;
	ourGPIO_Init(&GPIO_UART);
	
	MyUART_Init(USART1,9600,72000000);
	//MyUART_ActiveIT(USART1,Callback);
	
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
	MyTimer_ActiveIT(TIM4,2,Callback); // Timer 4 Channel 1 -> Broche PB6
	
	char Buffer1[7]; // Buffer pour la consigne
	int Consigne;
	
	char Buffer2[4]; // Buffer pour le rapport cyclique
	int Rapport_Cyclique_Auxi;
	
	while(1){
		ourADC_Start(&mon_ADC);
		Valeur_ADC = ourADC_Read(&mon_ADC);		
		if(0 <= Valeur_ADC && Valeur_ADC <= 81) Rapport_Cyclique = 2;
		if(82 <= Valeur_ADC && Valeur_ADC <= 4013) Rapport_Cyclique = 100*Valeur_ADC/4095;
		if(4014 <= Valeur_ADC && Valeur_ADC <= 4095) Rapport_Cyclique = 98;
		MyTimer_PWM_Set_Rapport_Cyclique(TIM2,2,Rapport_Cyclique);
		MyTimer_PWM_Set_Rapport_Cyclique(TIM3,1,100-Rapport_Cyclique);
		
		if(UART_Flag == 1){
			UART_Flag = 0;
			
			MyUART_SendString(USART1,"\nConsigne : ");
			Consigne = 1000 * Valeur_ADC_en_Volts(Valeur_ADC);
			Buffer1[6] = '\0';
			Buffer1[5] = 'V';
			
			for(int i=4;i>=2;i--){
				Buffer1[i] = (Consigne % 10) + '0'; // "+ '0'" pour que le chiffre soit converti en caractère
				Consigne /= 10;
			}
			
			Buffer1[1] = ',';
			Buffer1[0] = (Consigne % 10) + '0';
			
			for(int i=0;i<7;i++){
				MyUART_SendChar(USART1,Buffer1[i]);
			}
			
			MyUART_SendString(USART1,"\nPWM : ");
			Rapport_Cyclique_Auxi = Rapport_Cyclique;
			Buffer2[3] = '\0';
			Buffer2[2] = '%';
			
			Buffer2[1] = (Rapport_Cyclique_Auxi % 10) + '0';
			Rapport_Cyclique_Auxi /= 10;
			
			Buffer2[0] = (Rapport_Cyclique_Auxi % 10) + '0';
			
			for(int i=0;i<4;i++){
				MyUART_SendChar(USART1,Buffer2[i]);
			}
			
			MyUART_SendString(USART1,"\n");
		}
	}
}
