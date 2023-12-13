/*******************************************************************************
 * Copyright (C) 2023 by Suhas Srinivasa Reddy
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Suhas Srinivasa Reddy and the University of Colorado are not liable
 * for any misuse of this material.
 ******************************************************************************/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "MKL25Z4.h"
#include "pin_mux.h"

/* Standard C includes. */
#include "stdio.h"
#include "stdbool.h"

/* Custom driver includes. */
#include "tpm.h"
#include "sysclock.h"
#include "uart.h"
#include "motor_control.h"
#include "led.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Task priorities.
#define task_PRIORITY (configMAX_PRIORITIES - 1)
// Stack size.
#define stack_Size (512)
// Delay for turning actions (in milliseconds)
#define DELAY      (50)
// Startup light
#define STARTUP_LIGHT (0x888888)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void task_poll_BT(void *pvParameter);
static void task_motor_control(void *pvParameter);

TaskHandle_t motor_control_handle;
SemaphoreHandle_t xMutex; /* Handle to access mutex */
QueueHandle_t xQueue_sample;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

char BT_input;  // Global variable to acquire and use input from BT module

int main(void) {
	// Initialize system components
	Init_Sysclock();
	Init_UART0();
	Init_Motors();
	Init_LEDs();
	Init_TPM();

	// Create synchronization primitives
	xMutex = xSemaphoreCreateMutex();
	xQueue_sample = xQueueCreate(1, sizeof(uint8_t));

	// Send escape sequence to clear the terminal
	UART0_Transmit_String("\033[2J");
	// Move the cursor to the top-left corner
	UART0_Transmit_String("\033[H");
	UART0_Transmit_String("Initialized Wheels On The Go (BT Edition).....\n\r");

	// Set initial RGB color and start motors
	Set_RGB(STARTUP_LIGHT);
	Start_Motors(MEDIUM_SPEED, MEDIUM_SPEED);

	// Create tasks and start FreeRTOS scheduler
	xTaskCreate(task_poll_BT, "poll_BT", stack_Size,
	NULL, task_PRIORITY, NULL);
	xTaskCreate(task_motor_control, "poll_BT", stack_Size,
	NULL, task_PRIORITY, &motor_control_handle);
	vTaskStartScheduler();

	// The scheduler should not return, but in case of failure, return 0.
	while (1)
		;
	return 0;
}

/**
 * @brief Task to poll Bluetooth input.
 *
 * This task continuously polls for Bluetooth input and updates the global variable
 * `BT_input`. It also triggers the motor control task (`motor_control_handle`) to
 * process the received input.
 *
 * @param pvParameter Task parameters (unused in this case).
 */
static void task_poll_BT(void *pvParameter) {
	while (1) {
		// Take mutex to safely access shared resources
		xSemaphoreTake(xMutex, portMAX_DELAY);
		// Receive Bluetooth input
		BT_input = UART0_Receive_Byte();
		// Release the mutex
		xSemaphoreGive(xMutex);
		// Resume the motor control task to process the received input
		vTaskResume(motor_control_handle);
		// Introduce a delay to avoid excessive polling
		vTaskDelay(DELAY / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Task to manage motor control based on Bluetooth input.
 *
 * This task continuously waits for Bluetooth input (`BT_input`) and calls the
 * `Motor_Control` function to perform actions based on the received commands.
 * It then suspends itself until new input is received.
 *
 * @param pvParameter Task parameters (unused in this case).
 */
static void task_motor_control(void *pvParameter) {
	while (1) {
		// Take mutex to safely access shared resources
		xSemaphoreTake(xMutex, portMAX_DELAY);
		// Check if there is a valid Bluetooth input
		if (BT_input != '\0') {
			// Perform motor control based on the received command
			Motor_Control(BT_input);
		}
		// Release the mutex
		xSemaphoreGive(xMutex);
		// Suspend the task until new input is received
		vTaskSuspend(NULL);
	}
}
