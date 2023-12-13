/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "tpm.h"
#include "sysclock.h"
#include "uart.h"
#include "MKL25Z4.h"
#include "stdio.h"
#include "motor_control.h"
#include "stdbool.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void task_poll_BT(void *pvParameter);
static void task_state_machine(void *pvParameter);

TaskHandle_t task_motor_control;
SemaphoreHandle_t xMutex; /* Handle to access mutex */
QueueHandle_t xQueue_sample;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

char BT_input;
int main(void) {
	sysclock_init();
	Init_UART0();
	Init_TPM();
	xMutex = xSemaphoreCreateMutex();
	xQueue_sample = xQueueCreate(1, sizeof(uint8_t));

	// Send escape sequence to clear the terminal
	UART0_Transmit_String("\033[2J");
	// Move the cursor to the top-left corner
	UART0_Transmit_String("\033[H");
	UART0_Transmit_String("Initiating Wheels On The Go (BT Edition).....");

	Set_RGB(0x888888);
	Start_Motor(0xff, 0xff);

	xTaskCreate(task_poll_BT, "poll_BT", configMINIMAL_STACK_SIZE + 160,
	NULL, hello_task_PRIORITY, NULL);
	xTaskCreate(task_state_machine, "poll_BT", configMINIMAL_STACK_SIZE + 160,
	NULL, hello_task_PRIORITY, &task_motor_control);
	vTaskStartScheduler();

	while (1)
		;
	return 0;
}

static void task_poll_BT(void *pvParameter) {
	while (1) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		BT_input = UART0_Receive_Byte();
		xSemaphoreGive(xMutex);
		vTaskResume(task_motor_control);
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}

static void task_state_machine(void *pvParameter) {
	while (1) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		if (BT_input != '\0') {
			state_machine(BT_input);
		}
		xSemaphoreGive(xMutex);
		vTaskSuspend(NULL);
	}
}
