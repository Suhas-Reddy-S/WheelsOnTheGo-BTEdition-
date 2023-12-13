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
 * @file    led.c
 * @brief   Functions for controlling RGB LEDs using PWM signals.
 *
 * This file contains functions for initializing the necessary pins and configuring the Timer/PWM (TPM) module
 * to control RGB LEDs. It also includes a function for setting the color gradient of the RGB LEDs based on
 * PWM signals. The RGB LEDs are connected to specific pins on the microcontroller, and their intensity is adjusted
 * using calculated PWM values for each color component (red, green, and blue).
 *
 * Pin Configuration:
 * - Blue LED: Configured on TPM0_CH1 with Mux Alt 4 on Port D.
 * - Red LED: Configured on TPM2_CH0 with Mux Alt 3 on Port B.
 * - Green LED: Configured on TPM2_CH1 with Mux Alt 3 on Port B.
 *
 * Intensity Configuration:
 * - Red LED Intensity:   625
 * - Green LED Intensity: 1200
 * - Blue LED Intensity:  1200
 *
 * Color Gradient Calculation:
 * - The Set_RGB function calculates PWM values for each color component based on the provided color gradient.
 *   It adjusts the intensity of each LED to produce near-accurate colors.
 *
 * @author  Suhas Reddy S
 * @date    12th December 2023
 */
#include "led.h"
#include "MKL25Z4.h"

// Pin definitions for RGB LEDs
#define RED_PIN        (18)
#define GREEN_PIN      (19)
#define BLUE_PIN       (1)

// TPM channels for PWM control
#define TPM0_CH1       (4)
#define TPM2_CH0       (3)
#define TPM2_CH1       (3)

// TPM channels for PWM control (alternative names for clarity)
#define CH0            (0)
#define CH1            (1)

// Macro to extract the red component from a 24-bit color gradient
#define RED_GRADIANT(x) (((uint8_t)(((uint64_t)(x)) >> 16)) & 0xFF)

// Macro to extract the green component from a 24-bit color gradient
#define GREEN_GRADIANT(x) (((uint8_t)(((uint64_t)(x)) >> 8)) & 0xFF)

// Macro to extract the blue component from a 24-bit color gradient
#define BLUE_GRADIANT(x) ((uint8_t)((uint64_t)(x)) & 0xFF)

// Maximum value for the color gradient (8-bit)
#define MAX_GRADIANT    (0xFF)

// Intensity values for each color component
#define red_intensity   (625)
#define green_intensity (1200)
#define blue_intensity  (1200)

// Refer led.h file for function brief and description
void Init_LEDs(void) {
	// Blue TPM0_CH1, Mux Alt 4
	PORTD->PCR[BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_PIN] |= PORT_PCR_MUX(TPM0_CH1);

	// RED TPM2_CH0, Mux Alt 3
	PORTB->PCR[RED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_PIN] |= PORT_PCR_MUX(TPM2_CH0);

	// GREEN TPM2_CH1, Mux Alt 3
	PORTB->PCR[GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_PIN] |= PORT_PCR_MUX(TPM2_CH1);
}

// Refer led.h file for function brief and description
void Set_RGB(uint32_t color_gradiant) {
	TPM2->CONTROLS[CH0].CnV = red_intensity * RED_GRADIANT(color_gradiant)
			/ MAX_GRADIANT; // Adjusted intensity of each LED to produce near accurate color
	TPM2->CONTROLS[CH1].CnV = green_intensity * GREEN_GRADIANT(color_gradiant)
			/ MAX_GRADIANT;
	TPM0->CONTROLS[CH1].CnV = blue_intensity * BLUE_GRADIANT(color_gradiant)
			/ MAX_GRADIANT;
}
