#include "TIMER.h"

void (*PTF1) (void);
void (*PTF2) (void);
void (*PTF3) (void);
void (*PTF4) (void);


/*

I - Connectivity line devices: reset and clock control (RCC)

1) APB2ENR: APB2 peripheral clock enable register
Bit 11 TIM1EN: TIM1 Timer clock enable

2) APB1ENR: APB1 peripheral clock enable register
Bit 2 TIM4EN: Timer 4 clock enable
Bit 1 TIM3EN: Timer 3 clock enable
Bit 0 TIM2EN: Timer 2 clock enable

II - Timers

1) ARR: auto-reload register

2) PSC: prescaler

3) DIER: DMA/interrupt enable register
Bit 0 UIE: Update interrupt enable

4) SR: Status Register
Bit 0 UIF: Update interrupt flag

5) CCR: Capture/compare register

*/

void MyTimer_Base_Init(MyTimer_Struct_TypeDef * TimIN){
	if(TimIN->Timer == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	if(TimIN->Timer == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	if(TimIN->Timer == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	if(TimIN->Timer == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TimIN->Timer->ARR |= TimIN->ARR;
	TimIN->Timer->PSC |= TimIN->PSC;
}

void MyTimer_ActiveIT (TIM_TypeDef * Timer, char Prio, void ( * IT_function ) (void)){
	
	Timer->DIER |= TIM_DIER_UIE;
	
	if (Timer == TIM1){
		PTF1 = IT_function;
		NVIC_EnableIRQ(TIM1_UP_IRQn); // Autoriser la prise en compte de l'interruption
		NVIC_SetPriority(TIM1_UP_IRQn,Prio);
	} else if (Timer == TIM2){
		PTF2 = IT_function;
		NVIC_EnableIRQ(TIM2_IRQn); // Autoriser la prise en compte de l'interruption
		NVIC_SetPriority(TIM2_IRQn,Prio);
	} else if (Timer == TIM3){
		PTF3 = IT_function;
		NVIC_EnableIRQ(TIM3_IRQn); // Autoriser la prise en compte de l'interruption
		NVIC_SetPriority(TIM3_IRQn,Prio);
	} else if (Timer == TIM4){
		PTF4 = IT_function;
		NVIC_EnableIRQ(TIM4_IRQn); // Autoriser la prise en compte de l'interruption
		NVIC_SetPriority(TIM4_IRQn,Prio);
	} 
}

void TIM1_UP_IRQHandler(void){
	if(TIM1->SR & TIM_SR_UIF){
		TIM1->SR &= ~(TIM_SR_UIF); // Flag UIF remis � 0
		(*PTF1)();
	}
}

void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~(TIM_SR_UIF); // Flag UIF remis � 0
		(*PTF2)();
	}
}

void TIM3_IRQHandler(void){
	if(TIM3->SR & TIM_SR_UIF){
		TIM3->SR &= ~(TIM_SR_UIF); // Flag UIF remis � 0
		(*PTF3)();
	}
}

void TIM4_IRQHandler(void){
	if(TIM4->SR & TIM_SR_UIF){
		TIM4->SR &= ~(TIM_SR_UIF); // Flag UIF remis � 0
		(*PTF4)();
	}
}

void MyTimer_PWM_Init_Channel(TIM_TypeDef * Timer, char Channel){
	if (Channel == '1'){
		Timer->CCR1 |= TIM_CCR1_CCR1;
	}
	if (Channel == '2'){
		Timer->CCR2 |= TIM_CCR2_CCR2;
	}
	if (Channel == '3'){
		Timer->CCR3 |= TIM_CCR3_CCR3;
	}
	if (Channel == '4'){
		Timer->CCR4 |= TIM_CCR4_CCR4;
	}
}

void MyTimer_PWM_Set_Rapport_Cyclique(TIM_TypeDef * Timer, char Channel, int Rapport_Cyclique){
	if (Channel == '1'){
		Timer->CCR1 = Rapport_Cyclique * (1+Timer->ARR) / 100;
	}
	if (Channel == '2'){
		Timer->CCR2 = Rapport_Cyclique * (1+Timer->ARR) / 100;
	}
	if (Channel == '3'){
		Timer->CCR3 = Rapport_Cyclique * (1+Timer->ARR) / 100;
	}
	if (Channel == '4'){
		Timer->CCR4 = Rapport_Cyclique * (1+Timer->ARR) / 100;
	}
}