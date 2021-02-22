/*
 * Board Support Package (BSP.h) 
 * This file contains the prototypes needed for BSP.c
 */
 
#ifndef _BSP_H_
#define _BSP_H_

/* System Clock Tick [Hz] */
#define BSP_TICKS_PER_SEC 1000U

TickType_t tickDelay(const TickType_t delay);

/* Board Support Package */
void BSP_init(void);
void RTOS_init(void);

/* Stoplight 1 (S1) */
void redOnS1(void);
void yellowOnS1(void);
void greenOnS1(void);
void redOffS1(void);
void yellowOffS1(void);
void greenOffS1(void);

void redToggleS1(const TickType_t delay);
void yellowToggleS1(const TickType_t delay);
void greenToggleS1(const TickType_t delay);

void allOffS1(void);

void s1Task(void);

/* Stoplight 2 (S2) */
void redOnS2(void);
void yellowOnS2(void);
void greenOnS2(void);
void redOffS2(void);
void yellowOffS2(void);
void greenOffS2(void);

void redToggleS2(const TickType_t delay);
void yellowToggleS2(const TickType_t delay);
void greenToggleS2(const TickType_t delay);

void allOffS2(void);

void s2Task(void);

void allOffP(void);

/* Pedestrian 1 (PS1) */
void ps1On(void);
void ps1Off(void);
void ps1Task(void);

/* Pedestrian 2 (PS2) */
void ps2On(void);
void ps2Off(void);
void ps2Task(void);
	
/* Extern values */
extern int pCounter;
extern int sCounter; 

extern TaskHandle_t xTaskS1;
extern TaskHandle_t xTaskS2;
extern TaskHandle_t xTaskPS;

#endif // _BSP_H_
