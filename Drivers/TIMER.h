#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"

typedef struct {
	TIM_TypeDef * Timer; // TIM1 à TIM4
	unsigned short ARR;
	unsigned short PSC;
} MyTimer_Struct_TypeDef;
// Structure contenant un Timer et les valeurs entières respectives que l'on affecte aux registres ARR et PSC

void MyTimer_Base_Init(MyTimer_Struct_TypeDef * TimIN);
// Initialisation d'un Timer donné

void MyTimer_ActiveIT ( TIM_TypeDef * Timer, char Prio, void ( * IT_function ) (void));
// Activation d'une interruption
// La fonction MyTimer_Base_Init doit avoir été lancée avant de configurer une interruption
// Paramètres : - TIM_TypeDef * Timer : Timer concerné
//              - char Prio : Priorité allant de 0 à 15 (0 étant la plus grande priorité)

#define MyTimer_Base_Start(Timer) Timer->CR1 |= TIM_CR1_CEN
#define MyTimer_Base_Stop(Timer) Timer->CR1 &= ~(TIM_CR1_CEN)
// Macros permettant de démarrer ou d'arrêter un Timer donné

void MyTimer_PWM_Init_Channel(TIM_TypeDef * Timer, char Channel);
// Lancement d'une PWM sur un canal particulier d'un Timer donné
// Paramètres : - TIM_TypeDef * Timer : Timer concerné
//							- char Channel : canal du Timer (1, 2, 3 ou 4)

void MyTimer_PWM_Set_Rapport_Cyclique(TIM_TypeDef * Timer, char Channel, double Rapport_Cyclique);
// Affectation d'un rapport cyclique à une PWM d'un canal particulier d'un Timer donné

#endif
