#ifndef GPIO_H
#define GPIO_H
#include "stm32f10x.h"

typedef struct{
	GPIO_TypeDef * GPIO;
	char GPIO_pin; // Numéro de 0 à 15
	char GPIO_conf;
} ourGPIO_struct;
// Structure contenant un GPIO, le numéro du pin du GPIO et la configuration du GPIO

#define in_Floating 0x04
#define in_PullDown 0x08
#define in_PullUp 0x15
// Tableau 20 : Les modes de configuration Input pull-down et Input pull-up
// correspondent tous les deux à l'entier 1000 (chaque bit étant
// respectivement affecté aux registres : CNF1, CNF0, MODE1, MODE0),
// on met donc temporairement Pull-up à 15 et on le configurera correctement (à 8)
// lorsque l'on en aura besoin. La différence entre Pull-down et Pull-up 
// se jouera au niveau du registre PxODR.

#define in_Analog 0x00
#define out_Ppull 0x01
#define out_OD 0x05
#define altOut_Ppull 0x09
#define altOut_OD 0x13

// Les entiers ci-dessus sont la conversion en décimal des nombres
// binaires affectés aux registres : CNF1, CNF0, MODE1, MODE0.
// Ces nombres identifient les modes de configuration du GPIO.

void ourGPIO_Init( ourGPIO_struct * GPIOStructPtr);
// Initialisation d'un GPIO donné

int ourGPIO_Read( ourGPIO_struct * GPIO, char GPIO_pin );
// Indication de l'état d'un pin donné d'un GPIO donné
// La fonction retourne 1 si le pin spécifié est à l'état haut, sinon elle retourne 0

void ourGPIO_Set(ourGPIO_struct * GPIO, char GPIO_pin);
// Configuration du registre ODR pour le pin donné (cf. Tableau 20)

void ourGPIO_Reset(ourGPIO_struct * GPIO, char GPIO_pin);
// Réinitialisation du bit du registre ODR correspondant au pin donné (cf. Tableau 20)

void ourGPIO_Toggle(ourGPIO_struct * GPIO, char GPIO_pin);
// Inversion du bit du registre ODR correspondant au pin donné (cf. Tableau 20)

#endif