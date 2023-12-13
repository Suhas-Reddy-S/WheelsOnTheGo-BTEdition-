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
 * @file    pwm.c
 * @brief   PWM (Pulse-Width Modulation) control functions for RGB LEDs.
 *
 * This file contains functions for initializing the TPM module and setting the RGB LEDs
 * color gradient using PWM signals. It also includes functions for managing state transitions
 * and events based on touch input and the state machine.
 *
 * @author  Suhas Reddy S
 * @date    16th Oct 2023
 */

#include <tpm.h>

#define MASK(x) (1UL << (x))
#define PERIOD (4800)
#define RED_GRADIANT(x) (((uint8_t)(((uint64_t)(x)) >> 16)) & 0xFF)
#define GREEN_GRADIANT(x) (((uint8_t)(((uint64_t)(x)) >> 8)) & 0xFF)
#define BLUE_GRADIANT(x) ((uint8_t)((uint64_t)(x)) & 0xFF)
#define MAX_GRADIANT (0xFF)
#define red_intensity 625
#define green_intensity 1200
#define blue_intensity 1200

/**
 * @brief Initialize the TPM (Timer/PWM) module.
 *
 * This function initializes the TPM module to control RGB LEDs using PWM signals.
 * It configures the TPM module's settings, clock source, channels, and PWM characteristics.
 */
void Init_TPM(void) {
	// Enable clock to port D
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;

	// Blue TPM0_CH1, Mux Alt 4
	PORTD->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[1] |= PORT_PCR_MUX(4);

	// RED TPM1_CH0, Mux Alt 3
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[18] |= PORT_PCR_MUX(3);

	// GREEN TPM1_CH1, Mux Alt 3
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[19] |= PORT_PCR_MUX(3);

	PORTD->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[0] |= PORT_PCR_MUX(4);

	PORTD->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(4);

	PORTB->PCR[8] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[8] |= PORT_PCR_MUX(1);

	PORTB->PCR[9] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[9] |= PORT_PCR_MUX(1);

	PORTB->PCR[10] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[10] |= PORT_PCR_MUX(1);

	PORTB->PCR[11] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[11] |= PORT_PCR_MUX(1);

	PTB->PDDR |= MASK(8) | MASK(9) | MASK(10) | MASK(11);

	PTB->PCOR |= MASK(8) | MASK(9) | MASK(10) | MASK(11);

	// Enable Clock to TPM0 and TPM1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	//set clock source for tpm: 24 MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1));

	//load the counter and mod
	TPM0->MOD = PERIOD - 1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC = TPM_SC_PS(1000);
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 1 to edge-aligned low-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[1].CnV = 0;
	// Set channel 0 to edge-aligned low-true PWM
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[0].CnV = 0;
	// Set channel 0 to edge-aligned low-true PWM
	TPM0->CONTROLS[5].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[5].CnV = 0;

	// Start TPM
	TPM0->SC |= TPM_SC_CMOD(1);

	// Configure TPM2 and Load the counter and MOD
	TPM2->MOD = PERIOD - 1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM2->SC = TPM_SC_PS(1000);
	// Continue operation in debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 1 to edge-aligned low-true PWM
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM2->CONTROLS[0].CnV = 0;

	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM2->CONTROLS[1].CnV = 0;
	// Start TPM
	TPM2->SC |= TPM_SC_CMOD(1);
}

/**
 * @brief Set the color gradient for RGB LEDs.
 *
 * This function sets the color gradient for RGB LEDs by adjusting PWM duty cycles.
 * It calculates the PWM values for each color component (red, green, and blue) based
 * on the provided color gradient.
 *
 * @param color_gradiant The desired color gradient value to set the RGB LEDs.
 */
void Set_RGB(uint32_t color_gradiant) {
//	LOG("SETTING THE LEDS\n\r\n\r");
	TPM2->CONTROLS[0].CnV = red_intensity * RED_GRADIANT(color_gradiant)
			/ MAX_GRADIANT; // Adjusted intensity of each LED to produce near accurate color
	TPM2->CONTROLS[1].CnV = green_intensity * GREEN_GRADIANT(color_gradiant)
			/ MAX_GRADIANT;
	TPM0->CONTROLS[1].CnV = blue_intensity * BLUE_GRADIANT(color_gradiant)
			/ MAX_GRADIANT;
}
