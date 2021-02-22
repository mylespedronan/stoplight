#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp.h"

TaskHandle_t xTaskS1 = NULL;
TaskHandle_t xTaskS2 = NULL;
TaskHandle_t xTaskPS = NULL;

int pCounter = 0;
int sCounter = 0;

/* Stoplight 1 (S1) task */
void main_s1(void * pvParameters){
	while(1){
		s1Task();
		vTaskDelay(tickDelay(10));
	}
}

/* Stoplight 2 (S2) task */
void main_s2(void * pvParameters){
	while(1){
		s2Task();
		vTaskDelay(tickDelay(10));
	}
}

/* Pedestrian task */
void main_ped(void * pvParameters){		
	while(1){
		//ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
		
		if((pCounter == 1) && (sCounter == 1)){
			ps1Task();
		} else if ((pCounter == 2) && (sCounter == 2)){
			ps2Task();			
		}
		
		vTaskDelay(tickDelay(250));
	}
}


int main(){
	RTOS_init();
	BSP_init();								// Initialize Board Support Package (BSP)
	
	/* Initialize tasks */
	// Create task for Stoplight 1
	xTaskCreate(main_s1,					// Name of function to implement task
				"S1 Task", 					// Descriptive name of task
				configMINIMAL_STACK_SIZE, 	// Number of words to allocate for task's stack
				NULL,						// Value that will be passed into the created task as a parameter
				2,							// Priority level
				&xTaskS1);					// Used to pass a handle to the created task out of xTaskCreate()
	
	// Create task for Stoplight 2
	xTaskCreate(main_s2,
				"S2 Task",
				configMINIMAL_STACK_SIZE,
				NULL,
				2,
				&xTaskS2);
	
//	// Create task for Pedestrian S1 + S2
//	xTaskCreate(main_ped,
//				"P Task",
//				configMINIMAL_STACK_SIZE,
//				NULL,
//				3,
//				&xTaskPS);
				
	/* Startup of the FreeRTOS scheduler. The program should block here */
	vTaskStartScheduler();
	
	while(1){
	}
}
