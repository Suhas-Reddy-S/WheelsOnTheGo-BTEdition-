/*******************************************************************************
 * Copyright (C) 2023 by Suhas Srinivasa Reddy
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Suhas Srinivasa Reddy and the University of Colorado are not liable
 * for any misuse of this material.
 ******************************************************************************/

/**
 * @file    motor_control.c
 * @brief   Functions for controlling motors and managing robot movement.
 *
 * This file contains functions for initializing and controlling the motors of a robot.
 * It configures PWM signals to control motor speed and GPIO pins to control the direction
 * of motor rotation. The module provides functions to move the robot forward, backward,
 * turn left, turn right, and stop. It also interfaces with the LED module to indicate
 * the robot's movement status using RGB LEDs.
 *
 * Motor Configuration:
 * - Motor A: Connected to TPM0_CH0 (Pin D0) for PWM control and PTB10/PTB11 for direction control.
 * - Motor B: Connected to TPM0_CH5 (Pin D5) for PWM control and PTB8/PTB9 for direction control.
 *
 * Direction Control:
 * - PTB8/PTB9: Motor A direction control pins.
 * - PTB10/PTB11: Motor B direction control pins.
 *
 * @author  Suhas Reddy S
 * @date    12th Dec 2023
 */
#include "motor_control.h"
#include "MKL25Z4.h"
#include "tpm.h"
#include "stdbool.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

// Bit mask macro for setting a specific bit
#define MASK(x) (1UL << (x))

// RGB color values
#define GREEN   (0xFF00)
#define RED     (0xFF0000)
#define YELLOW  (0xFFFF00)
#define CYAN    (0xFFFF)

// Motor A PWM pin and associated TPM channel
#define MOTORA_PWM_PIN  (0)
#define TPM0_CH0        (4)

// Motor B PWM pin and associated TPM channel
#define MOTORB_PWM_PIN  (5)
#define TPM0_CH5        (4)

// GPIO mode
#define GPIO            (1)

// Motor A control pins
#define MOTORA_CW       (11)
#define MOTORA_CCW      (10)

// Motor B control pins
#define MOTORB_CW       (8)
#define MOTORB_CCW      (9)

// TPM channels for Motor A and Motor B
#define CH0             (0)
#define CH5             (5)

// Delay for turning actions (in milliseconds)
#define DELAY           (500)


void forward(void);
void backward(void);
void right(void);
void left(void);
void stop(void);

// flag to track stop
bool isstop = true;

// Refer motor_control.h file for function brief and description
void Init_Motors(void) {
	// Enable clock to Port B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;

	// PWM control for Motot A
	PORTD->PCR[MOTORA_PWM_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[MOTORA_PWM_PIN] |= PORT_PCR_MUX(TPM0_CH0);

	// PWM control for Motor B
	PORTD->PCR[MOTORB_PWM_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[MOTORB_PWM_PIN] |= PORT_PCR_MUX(TPM0_CH5);

	// Enable pins 8, 9, 10, and 11 as GPIOs to control the direction of rotation of the robots
	// Motor B direction control pins
	// Clockwise
	PORTB->PCR[MOTORB_CW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[MOTORB_CW] |= PORT_PCR_MUX(GPIO);

	// Counter-Clockwise
	PORTB->PCR[MOTORB_CCW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[MOTORB_CCW] |= PORT_PCR_MUX(GPIO);

	// Motor A direction control pins
	// Counter-Clockwise
	PORTB->PCR[MOTORA_CCW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[MOTORA_CCW] |= PORT_PCR_MUX(GPIO);

	// Clockwise
	PORTB->PCR[MOTORA_CW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[MOTORA_CW] |= PORT_PCR_MUX(GPIO);

	// Set the direction of the pins as outputs
	PTB->PDDR |= MASK(
			MOTORB_CW) | MASK(MOTORB_CCW) | MASK(MOTORA_CCW) | MASK(MOTORA_CW);

	// Clear the direction control pins
	PTB->PCOR |= MASK(
			MOTORB_CW) | MASK(MOTORB_CCW) | MASK(MOTORA_CCW) | MASK(MOTORA_CW);
}

// Refer motor_control.h file for function brief and description
void Start_Motors(uint16_t speed_a, uint16_t speed_b) {
	TPM0->CONTROLS[CH5].CnV = speed_a;  // Higher the value lower the speed
	TPM0->CONTROLS[CH0].CnV = speed_b;
}

// Refer motor_control.h file for function brief and description
void Motor_Control(char ch) {
	if (ch == '1') {
		// Move forward
		Set_RGB(GREEN);
		forward();
		isstop = true;
	} else if (ch == '2') {
		// Toggle between stop and backward movement
		Set_RGB(RED);
		if (isstop) {
			stop();
			isstop = false;
		} else {
			backward();
			isstop = true;
		}
	} else if (ch == '3') {
		// Turn right with a brief delay and then stop
		Set_RGB(CYAN);
		right();
		vTaskDelay(DELAY / portTICK_PERIOD_MS);
		stop();
		isstop = true;
	} else if (ch == '4') {
		// Turn left with a brief delay and then stop
		Set_RGB(YELLOW);
		left();
		vTaskDelay(DELAY / portTICK_PERIOD_MS);
		stop();
		isstop = true;
	}
}

/**
 * @brief Move the robot forward.
 *
 * This function sets the motor directions and transmits a message indicating
 * that the robot is moving forward using UART communication.
 */
void forward(void) {
	UART0_Transmit_String("Moving Forward...\n\r");
	PTB->PSOR |= MASK(MOTORB_CCW);  // PSOR sets the pin high
	PTB->PSOR |= MASK(MOTORA_CW);
	PTB->PCOR |= MASK(MOTORB_CW);   // PCOR sets the pin low
	PTB->PCOR |= MASK(MOTORA_CCW);
}

/**
 * @brief Move the robot backward.
 *
 * This function sets the motor directions and transmits a message indicating
 * that the robot is moving backward using UART communication.
 */
void backward(void) {
	UART0_Transmit_String("Moving Backward...\n\r");
	PTB->PCOR |= MASK(MOTORB_CCW);
	PTB->PCOR |= MASK(MOTORA_CW);
	PTB->PSOR |= MASK(MOTORB_CW);
	PTB->PSOR |= MASK(MOTORA_CCW);
}

/**
 * @brief Turn the robot to the right.
 *
 * This function sets the motor directions and transmits a message indicating
 * that the robot is turning right using UART communication.
 */
void right(void) {
	UART0_Transmit_String("Turning Right...\n\r");
	PTB->PSOR |= MASK(MOTORB_CCW);
	PTB->PCOR |= MASK(MOTORA_CW);
	PTB->PCOR |= MASK(MOTORB_CW);
	PTB->PSOR |= MASK(MOTORA_CCW);
}

/**
 * @brief Turn the robot to the left.
 *
 * This function sets the motor directions and transmits a message indicating
 * that the robot is turning left using UART communication.
 */
void left(void) {
	UART0_Transmit_String("Turning Left...\n\r");
	PTB->PCOR |= MASK(MOTORB_CCW);
	PTB->PSOR |= MASK(MOTORA_CW);
	PTB->PSOR |= MASK(MOTORB_CW);
	PTB->PCOR |= MASK(MOTORA_CCW);
}

/**
 * @brief Stop the robot.
 *
 * This function stops both motors and transmits a message indicating
 * that the robot has stopped using UART communication.
 */
void stop(void) {
	UART0_Transmit_String("Stopped...\n\r");
	PTB->PCOR |= MASK(MOTORB_CCW);
	PTB->PCOR |= MASK(MOTORA_CW);
	PTB->PCOR |= MASK(MOTORB_CW);
	PTB->PCOR |= MASK(MOTORA_CCW);
}
