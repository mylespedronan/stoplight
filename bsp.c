/*
 * Board Support Package (BSP.c) 
 * This file contains the functions needed to control the 
 * I/O functions of the Tiva Launchpad.
 */
 
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp.h"
#include "TM4C123GH6PM.h"

/* Define Macros for GPIO pins 1-3 */
#define PIN0	(1U << 0)
#define PIN1	(1U << 1)
#define PIN2	(1U << 2)
#define PIN3	(1U << 3)
#define PIN4	(1U << 4)

#define BTN_SW1 (1U << 4)
#define BTN_SW2 (1U << 0)
#define GPIO_LOCK_KEY	(0x4C4F434B)

static SemaphoreHandle_t xMutex;

void GPIOF_Handler(void){
//	BaseType_t xHigherPriorityTaskWoken;
//	
//	xHigherPriorityTaskWoken = pdFALSE;	
	
	/* GPIO Raw Interrupt Status set to unblock semaphore for SW1 */
	if((GPIOF_AHB->RIS & BTN_SW1) != 0U){
		//vTaskNotifyGiveFromISR(xTaskPS, &xHigherPriorityTaskWoken);
		if(pCounter != 1){
			pCounter = 1;
		}
	}
	
	/* GPIO Raw Interrupt Status set to unblock semaphore for SW2 */
	if((GPIOF_AHB->RIS & BTN_SW2) != 0U){
		//vTaskNotifyGiveFromISR(xTaskPS, &xHigherPriorityTaskWoken);
		if(pCounter != 2){
			pCounter = 2;
		}
	}
	
	GPIOF_AHB->ICR = 0xFFU;			/* Clear GPIO Interrupt Clear (GPIOICR) register */ 
	
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void RTOS_init(void){
	SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC );
	
	/* Set interrupt's priority */
	/* NOTE: the lowest interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY
	 * and maximum interrupt priority is set by config_LIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
	*/
	
	NVIC_SetPriority(SysTick_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2);	
	NVIC_SetPriority(GPIOF_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);	
	
	NVIC_EnableIRQ(GPIOF_IRQn);			/* Enable Interrupt */
}

void BSP_init(void){
	/* GPIO Control */	
	SYSCTL->RCGCGPIO |= ((1U << 5) | (1U << 4) | (1U << 3));		/* Enable clock for PORTE, PORTD, PORTF */
	SYSCTL->GPIOHBCTL |= ((1U << 5) | (1U << 4) | (1U << 3));		/* Enable High Speed Bus for PORTE, PORTD, PORTF */
	
	/* Set direction of GPIO Port D and Port E as output */
	GPIOD_AHB->DIR |= (PIN0 | PIN1 | PIN2 | PIN3);			
	GPIOE_AHB->DIR |= (PIN1 | PIN2 | PIN3 | PIN4);
	
	/* Set digital enable of GPIO Port D and Port E as output */
	GPIOD_AHB->DEN |= (PIN0 | PIN1 | PIN2 | PIN3);			
	GPIOE_AHB->DEN |= (PIN1 | PIN2 | PIN3 | PIN4);
	
	/* GPIOF On-board switches */
	GPIOF_AHB->LOCK = GPIO_LOCK_KEY;
	GPIOF_AHB->CR |= BTN_SW2;
	
	GPIOF_AHB->DIR &= ~(BTN_SW1 | BTN_SW2);					/* Set direction to input */
	GPIOF_AHB->DEN |= (BTN_SW1 | BTN_SW2);					/* Digitally enable SW1 and SW2 */
	GPIOF_AHB->PUR |= (BTN_SW1 | BTN_SW2);					/* Set Pull-up registers for SW1 and SW2 */

	/* Configure interrupts for on-board switches */
	GPIOF_AHB->IS  &= ~(BTN_SW1 | BTN_SW2);					/* GPIO Interrupt Sense set to edge-sensitive */
	GPIOF_AHB->IBE &= ~(BTN_SW1 | BTN_SW2);					/* GPIO Interrupt Both Edges set to controlled by GPIOIEV */
	GPIOF_AHB->IEV &= ~(BTN_SW1 | BTN_SW2);					/* GPIO Interrupt Event set to falling edge/low */
	GPIOF_AHB->IM  |= (BTN_SW1 | BTN_SW2);					/* GPIO Interrupt Mask set to interrupt controller */
	
	/* Create a mutex for the stoplights */
	xMutex = xSemaphoreCreateMutex();
	
}

/* Function to set tick delay for a task */
TickType_t tickDelay(const TickType_t delay){
	TickType_t xDelay = pdMS_TO_TICKS(delay);
	
	return xDelay;
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

void redToggleS1(const TickType_t delay){
	GPIOD_AHB->DATA_Bits[PIN1] |= PIN1;
	vTaskDelay(tickDelay(delay));
	GPIOD_AHB->DATA_Bits[PIN1] = 0U;	
}

void yellowToggleS1(const TickType_t delay){
	GPIOD_AHB->DATA_Bits[PIN2] |= PIN2;
	vTaskDelay(tickDelay(delay));
	GPIOD_AHB->DATA_Bits[PIN2] = 0U;
}

void greenToggleS1(const TickType_t delay){
	GPIOD_AHB->DATA_Bits[PIN3] |= PIN3;
	vTaskDelay(tickDelay(delay));
	GPIOD_AHB->DATA_Bits[PIN3] = 0U;
}

void allOffS1(void){
	redOffS1();
	yellowOffS1();
	greenOffS1();
}

void s1Task(void){
	if(pCounter == 1 && sCounter == 1){
		ps1Task();
	} else if(pCounter != 1){
		if (xMutex != NULL){
			if( xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE){
							
				redOnS2();
				
				greenToggleS1(5000);
				yellowToggleS1(1000);
				redOnS1();
				
				redOffS2();

				xSemaphoreGive(xMutex);
				
			}
		}		
	}
	sCounter = 2;
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

void redToggleS2(const TickType_t delay){
	GPIOE_AHB->DATA_Bits[PIN1] |= PIN1;
	vTaskDelay(tickDelay(delay));
	GPIOE_AHB->DATA_Bits[PIN1] = 0U;
}

void yellowToggleS2(const TickType_t delay){
	GPIOE_AHB->DATA_Bits[PIN2] |= PIN2;
	vTaskDelay(tickDelay(delay));
	GPIOE_AHB->DATA_Bits[PIN2] = 0U;
}

void greenToggleS2(const TickType_t delay){
	GPIOE_AHB->DATA_Bits[PIN3] |= PIN3;
	vTaskDelay(tickDelay(delay));
	GPIOE_AHB->DATA_Bits[PIN3] = 0U;
}

void allOffS2(void){
	redOffS2();
	yellowOffS2();
	greenOffS2();
}

void s2Task(void){
	if(pCounter == 2 && sCounter == 2){
		ps2Task();
	} else if(pCounter != 2){
		if (xMutex != NULL){
			if( xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE){	
				
				redOnS1();
				
				greenToggleS2(5000);
				yellowToggleS2(1000);
				redOnS2();

				redOffS1();
				
				xSemaphoreGive(xMutex);	
				
				//vTaskDelay(tickDelay(250));
			}
		}
	}
	sCounter = 1;
}

void allOffP(void){
	allOffS1();
	allOffS2();
}

/* Pedestrian 1 (PS1) */
void ps1On(void){
	GPIOD_AHB->DATA_Bits[PIN0] |= PIN0;
}

void ps1Off(void){
	GPIOD_AHB->DATA_Bits[PIN0] = 0U;
}

void ps1Task(void){
	if (xMutex != NULL){
		if( xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE){
			allOffP();
			
			redOnS2();
			ps1On();
			greenToggleS1(10000);
			yellowToggleS1(1000);
			ps1Off();
			redOffS2();
			
			pCounter = 0;
			sCounter = 0;
			
			xSemaphoreGive(xMutex);
		}
	}
}

/* Pedestrian 2 (PS2) */
void ps2On(void){
	GPIOE_AHB->DATA_Bits[PIN4] |= PIN4;
}

void ps2Off(void){
	GPIOE_AHB->DATA_Bits[PIN4] = 0U;
}

void ps2Task(void){
	if (xMutex != NULL){
		if( xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE){
			allOffP();
			
			redOnS1();
			ps2On();
			greenToggleS2(10000);
			yellowToggleS2(1000);
			ps2Off();
			redOffS1();
			
			pCounter = 0;
			sCounter = 0;
			
			xSemaphoreGive(xMutex);
		}
	}
}
