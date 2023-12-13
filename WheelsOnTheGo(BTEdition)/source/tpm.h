// tpm.h

#ifndef TPM_H
#define TPM_H

#include <MKL25Z4.h>

/**
 * @brief Initialize the TPM (Timer/PWM) module for controlling RGB LEDs.
 *
 * This function initializes the TPM module by configuring its settings, clock source, channels,
 * and PWM characteristics for controlling RGB LEDs using PWM signals.
 */
void Init_TPM(void);

#endif // TPM_H
