#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"

/* Stoplight 1 (S1) red light task */
void main_s1(void * pvParameters){
	while(1){
		redOnS1();
		redOffS1();
	}
}

/* Stoplight 1 (S1) yellow light task */
void main_s2(void * pvParameters){
	while(1){
		yellowOnS1();
		yellowOffS1();
	}
}

/* Stoplight 1 (S1) green light task */
void main_s3(void * pvParameters){
	while(1){
		greenOnS1();
		greenOffS1();
	}
}

int main(){
	BSP_init();								// Initialize Board Support Package (BSP)
	
	/* Initialize tasks */
	// Create task for Stoplight 1
	xTaskCreate(main_s1,					// Name of function to implement task
				"S1 Task", 					// Descriptive name of task
				configMINIMAL_STACK_SIZE, 	// Number of words to allocate for task's stack
				NULL,						// Value that will be passed into the created task as a parameter
				1,							// Priority level
				NULL);						// Used to pass a handle to the created task out of xTaskCreate()
	
	// Create task for Stoplight 2
	xTaskCreate(main_s2,
				"S2 Task",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL);
	
	// Create task for Pedestrian
	xTaskCreate(main_s3,
				"P Task",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL);
	
	/* Startup of the FreeRTOS scheduler. The program should block here */
	vTaskStartScheduler();
	
	while(1){
	}
}
