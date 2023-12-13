/**
 * @file    pwm.h
 * @brief   Header file for PWM (Pulse-Width Modulation) control functions and declarations.
 *
 * This header file provides declarations and function prototypes for controlling RGB LEDs
 * using PWM signals. It includes functions for initializing the TPM module, setting RGB
 * LED colors with gradients, and managing state transitions based on touch input and a state machine.
 *
 * @author  Suhas Reddy S
 * @date    16th Oct 2023
 */

#ifndef _LED_H_
#define _LED_H_

#include <MKL25Z4.h>
#include <stdbool.h>

/**
 * @brief Initialize the TPM (Timer/PWM) module for controlling RGB LEDs.
 *
 * This function initializes the TPM module by configuring its settings, clock source, channels,
 * and PWM characteristics for controlling RGB LEDs using PWM signals.
 */
void Init_TPM(void);

/**
 * @brief Set the color gradient for RGB LEDs using PWM.
 *
 * This function sets the color gradient for RGB LEDs by adjusting the PWM duty cycles for each color
 * component (red, green, and blue) based on the provided color gradient value.
 *
 * @param color_gradiant The desired color gradient value for setting RGB LED colors.
 */
void Set_RGB(uint32_t color_gradiant);

#endif /* _PWM_H_ */
