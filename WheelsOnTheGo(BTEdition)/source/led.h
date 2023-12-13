// led.h

#ifndef LED_H
#define LED_H

#include "stdint.h"

/**
 * @brief Initializes the pins for controlling RGB LEDs on the microcontroller.
 *
 * This function configures the necessary port pins to control RGB LEDs connected to the microcontroller.
 * It sets up the pin configurations for each color (Red, Green, and Blue) by configuring the Port Control Register (PCR)
 * and selecting the appropriate alternate function for each pin. The configuration is specific to the TPM (Timer/PWM) channels
 * associated with each color.
 *
 * Pin Configuration:
 * - Blue LED: Configured on TPM0_CH1 with Mux Alt 4 on Port D.
 * - Red LED: Configured on TPM2_CH0 with Mux Alt 3 on Port B.
 * - Green LED: Configured on TPM2_CH1 with Mux Alt 3 on Port B.
 *
 * @note Ensure that the corresponding Timer/PWM modules (TPM0 and TPM2) are initialized separately for PWM control.
 *       The user should initialize TPM0 and TPM2 with appropriate settings to control the LED intensity.
 */
void Init_LEDs(void);

/**
 * @brief Set the color gradient for RGB LEDs using PWM.
 *
 * This function sets the color gradient for RGB LEDs by adjusting the PWM duty cycles for each color
 * component (red, green, and blue) based on the provided color gradient value.
 *
 * @param color_gradient The desired color gradient value for setting RGB LED colors.
 */
void Set_RGB(uint32_t color_gradient);

#endif // LED_H
