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
 * @file    tpm.c
 * @brief   Functions for initializing and configuring the TPM (Timer/PWM) module.
 *
 * This file contains functions for initializing and configuring the TPM module to control RGB LEDs
 * using PWM signals. The TPM module is configured with a specific period, clock source, and prescaler
 * to generate PWM signals for controlling the intensity of RGB LEDs. The module is set to operate in
 * debug mode, and multiple channels are configured for edge-aligned low-true PWM.
 *
 * Configuration:
 * - Clock Source: 24 MHz
 * - Prescaler: Divide by 1000
 * - Period: 4800 (adjustable as needed)
 *
 * Channels Configuration:
 * - TPM0_CH0 and TPM0_CH1: Edge-aligned low-true PWM
 * - TPM0_CH5: Edge-aligned low-true PWM
 * - TPM2_CH0 and TPM2_CH1: Edge-aligned low-true PWM
 *
 * @author  Suhas Reddy S
 * @date    12th Dec 2023
 */
#include "tpm.h"

// Period value for the TPM module
#define PERIOD      (4800)

// TPM channels
#define CH0         (0)
#define CH1         (1)
#define CH5         (5)

// Numeric constants
#define ZERO        (0)
#define ONE         (1)

// Debug mode configuration for the TPM module
#define DEBUG_MODE  (3)

// Time delay constant (in milliseconds)
#define MS1000      (1000)

// Refer tpm.h file for function brief and description
void Init_TPM(void) {
	// Enable Clock to TPM0 and TPM1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	//set clock source for tpm: 24 MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(ONE));

	//load the counter and mod
	TPM0->MOD = PERIOD - 1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC = TPM_SC_PS(MS1000);
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(DEBUG_MODE);
	// Set channel 1 to edge-aligned low-true PWM
	TPM0->CONTROLS[CH1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[CH1].CnV = ZERO;
	// Set channel 0 to edge-aligned low-true PWM
	TPM0->CONTROLS[CH0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[CH0].CnV = ZERO;
	// Set channel 5 to edge-aligned low-true PWM
	TPM0->CONTROLS[CH5].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[CH5].CnV = ZERO;
	// Start TPM0
	TPM0->SC |= TPM_SC_CMOD(ONE);

	// Configure TPM2 and Load the counter and MOD
	TPM2->MOD = PERIOD - 1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM2->SC = TPM_SC_PS(MS1000);
	// Continue operation in debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(DEBUG_MODE);
	// Set channel 1 to edge-aligned low-true PWM
	TPM2->CONTROLS[CH0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM2->CONTROLS[CH0].CnV = ZERO;

	TPM2->CONTROLS[CH1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM2->CONTROLS[CH1].CnV = ZERO;
	// Start TPM
	TPM2->SC |= TPM_SC_CMOD(ONE);
}
