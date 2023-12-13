/**
 * @file    uart.c
 * @brief   UART (Universal Asynchronous Receiver-Transmitter) communication functions.
 *
 * This file contains functions for initializing the UART module, transmitting strings,
 * and managing UART communication. It includes functions for initializing UART0,
 * sending null-terminated strings, and transmitting arrays of bytes.
 *
 * @author  Suhas Reddy S
 * @date    17th November 2023
 */

#include <MKL25Z4.h>
#include "uart.h"
#include "sysclock.h"
#include "stdio.h"

#define BAUD_RATE 	(115200)
#define UART_OVERSAMPLE_RATE  (16)
#define DATA_BITS  (0)     // 1 for 8 bits and 0 for 9 bits
#define STOP_BITS (0)      // 0 for 1 stop bit and 1 for 2 stop bits
#define PARITY_ENABLE (0)  // 1 to enable parity
#define PARITY_TYPE (1)    // 1 for odd and 0 for even
#define SBR_MSBYTE (8)
#define ZERO (0)
#define ONE (1)
#define TWO (2)

/**
 * @brief   Initialize UART0 for serial communication.
 *
 * This function configures and initializes the UART0 module for serial communication.
 * It sets up the necessary clock gating, pins for Rx and Tx, baud rate, data format,
 * and enables the UART receiver and transmitter. Additionally, it configures interrupts
 * for UART receive and clears any error flags.
 *
 * @note    The function uses a 24 MHz clock source for UART0 and supports an 8-bit data format,
 *          one stop bit, and optional parity.
 * @note    The baud rate is calculated based on the system clock frequency and the specified
 *          baud rate constant.
 * @note    The function enables UART interrupts for receive and initializes the NVIC.
 *
 * Author Prof. Dean
 *
 * Modified by Suhas Srinivasa Reddy
 *
 */
void Init_UART0() {
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(TWO); // Rx
	PORTA->PCR[2] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(TWO); // Tx

	PORTD->PCR[6] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(3); // Rx
	PORTD->PCR[7] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(3); // Tx

	// Set baud rate and oversampling ratio
	sbr =
			(uint16_t) ((SYSCLOCK_FREQUENCY)
					/ (BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> SBR_MSBYTE);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-ONE);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |=
	UART0_BDH_RXEDGIE(ZERO) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(ZERO);

	// Don't enable loopback mode, use 8 data bit mode, use parity and odd parity type
	UART0->C1 = UART0_C1_LOOPS(
			ZERO) | UART0_C1_M(DATA_BITS) | UART0_C1_PE(PARITY_ENABLE); //| UART0_C1_PT(PARITY_TYPE);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(ZERO) | UART0_C3_ORIE(ZERO)| UART0_C3_NEIE(ZERO)
	| UART0_C3_FEIE(ZERO) | UART0_C3_PEIE(ZERO);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(
			ONE) | UART0_S1_NF(ONE) | UART0_S1_FE(ONE) | UART0_S1_PF(ONE);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(ZERO) | UART0_S2_RXINV(ZERO);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(ONE) | UART0_C2_TE(ONE);

}

// refer uart.h for function brief
void UART0_Transmit_String(const char *str) {
	while (*str != '\0') {
		UART0_Transmit_Char(*str);
		str++;
	}
}

// refer uart.h for function brief
void UART0_Transmit_Char(char val) {
	while (!(UART0->S1 & UART_S1_TDRE_MASK));
	UART0->D = val;
}

// refer uart.h for function brief
char UART0_Receive_Byte(void) {
	while (!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}
