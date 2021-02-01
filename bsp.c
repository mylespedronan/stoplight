/*
 * Board Support Package (BSP.c) 
 * This file contains the functions needed to control the 
 * I/O functions of the Tiva Launchpad.
 */
 
#include "bsp.h"
#include "TM4C123GH6PM.h"

/* Define Macros for GPIO pins 1-3 */
#define PIN1	(1U << 1)
#define PIN2	(1U << 2)
#define PIN3	(1U << 3)

void BSP_init(void){
	/* GPIO Control */
	SYSCTL->RCGCGPIO |= ((1U << 4) | (1U << 3));			/* Enable clock for PORTE and PORTD */
	SYSCTL->GPIOHBCTL |= ((1U << 4) | (1U << 3));			/* Enable High Speed Bus for PORTE and PORTD */
	
	/* Set direction of GPIO Port D and Port E as output */
	GPIOD_AHB->DIR |= ((PIN1) | (PIN2) | (PIN3));			
	GPIOE_AHB->DIR |= ((PIN1) | (PIN2) | (PIN3));
	
	/* Set digital enable of GPIO Port D and Port E as output */
	GPIOD_AHB->DEN |= ((PIN1) | (PIN2) | (PIN3));
	GPIOE_AHB->DEN |= ((PIN1) | (PIN2) | (PIN3));	
}

/* Stoplight 1 (S1) */
void redOnS1(void){
	GPIOD_AHB->DATA_Bits[PIN1] |= PIN1;
}

void yellowOnS1(void){
	GPIOD_AHB->DATA_Bits[PIN2] |= PIN2;
}

void greenOnS1(void){
	GPIOD_AHB->DATA_Bits[PIN3] |= PIN3;
}

void redOffS1(void){
	GPIOD_AHB->DATA_Bits[PIN1] = 0U;
}

void yellowOffS1(void){
	GPIOD_AHB->DATA_Bits[PIN2] = 0U;
}

void greenOffS1(void){
	GPIOD_AHB->DATA_Bits[PIN3] = 0U;
}

/* Stoplight 2 (S2) */
void redOnS2(void){
	GPIOE_AHB->DATA_Bits[PIN1] |= PIN1;
}

void yellowOnS2(void){
	GPIOE_AHB->DATA_Bits[PIN2] |= PIN2;
}

void greenOnS2(void){
	GPIOE_AHB->DATA_Bits[PIN3] |= PIN3;
}

void redOffS2(void){
	GPIOE_AHB->DATA_Bits[PIN1] = 0U;
}

void yellowOffS2(void){
	GPIOE_AHB->DATA_Bits[PIN2] = 0U;
}

void greenOffS2(void){
	GPIOE_AHB->DATA_Bits[PIN3] = 0U;
}

