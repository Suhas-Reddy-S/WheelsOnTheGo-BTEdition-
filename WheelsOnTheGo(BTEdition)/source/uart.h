#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @file    uart.h
 * @brief   UART (Universal Asynchronous Receiver-Transmitter) module functions for serial communication.
 *
 * This header file contains functions to initialize UART0 for serial communication,
 * transmit null-terminated strings, transmit individual characters, and receive bytes.
 *
 * @author  [Your Name]
 * @date    [Current Date]
 */

/**
 * @brief Initialize UART0 for serial communication.
 *
 * This function initializes the UART0 module for serial communication.
 * It configures the necessary settings such as baud rate, data format, and interrupts.
 * Call this function before using other UART functions.
 */
void Init_UART0(void);

/**
 * @brief Send a null-terminated string over UART0.
 *
 * This function sends the specified null-terminated string over UART0.
 * The string is transmitted character by character until the null terminator is encountered.
 *
 * @param str The null-terminated string to be transmitted.
 */
void UART0_Transmit_String(const char *str);

/**
 * @brief Transmit a single character over UART0.
 *
 * This function transmits a single character over UART0.
 *
 * @param val The character to be transmitted.
 */
void UART0_Transmit_Char(char val);

/**
 * @brief Receive a byte from UART0.
 *
 * This function receives a byte from UART0.
 *
 * @return The received byte.
 */
char UART0_Receive_Byte(void);

#endif // UART_H
