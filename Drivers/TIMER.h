#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"

typedef struct {
	TIM_TypeDef * Timer; // TIM1 � TIM4
	unsigned short ARR;
	unsigned short PSC;
} MyTimer_Struct_TypeDef;
// Structure contenant un Timer et les valeurs enti�res respectives que l'on affecte aux registres ARR et PSC

void MyTimer_Base_Init(MyTimer_Struct_TypeDef * TimIN);
// Initialisation d'un Timer donn�

void MyTimer_ActiveIT ( TIM_TypeDef * Timer, char Prio, void ( * IT_function ) (void));
// Activation d'une interruption
// La fonction MyTimer_Base_Init doit avoir �t� lanc�e avant de configurer une interruption
// Param�tres : - TIM_TypeDef * Timer : Timer concern�
//              - char Prio : Priorit� allant de 0 � 15 (0 �tant la plus grande priorit�)

#define MyTimer_Base_Start(Timer) Timer->CR1 |= TIM_CR1_CEN
#define MyTimer_Base_Stop(Timer) Timer->CR1 &= ~(TIM_CR1_CEN)
// Macros permettant de d�marrer ou d'arr�ter un Timer donn�

void MyTimer_PWM_Init_Channel(TIM_TypeDef * Timer, char Channel);
// Lancement d'une PWM sur un canal particulier d'un Timer donn�
// Param�tres : - TIM_TypeDef * Timer : Timer concern�
//							- char Channel : canal du Timer (1, 2, 3 ou 4)

void MyTimer_PWM_Set_Rapport_Cyclique(TIM_TypeDef * Timer, char Channel, double Rapport_Cyclique);
// Affectation d'un rapport cyclique � une PWM d'un canal particulier d'un Timer donn�

#endif
