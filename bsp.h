/*
 * Board Support Package (BSP.h) 
 * This file contains the prototypes needed for BSP.c
 */
 
#ifndef _BSP_H_
#define _BSP_H_

/* Board Support Package */
void BSP_init(void);

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

void s2Task(void);

//void vApplicationIdleHook(void);

/* Extern values */
//extern SemaphoreHandle_t xMutex;

#endif // _BSP_H_
