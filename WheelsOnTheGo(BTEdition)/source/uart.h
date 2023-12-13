#ifndef UART_H
#define UART_H

#include <stdint.h>

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
 * @brief Transmit an array of bytes over UART0.
 *
 * This function transmits an array of bytes over UART0.
 *
 * @param data The array of bytes to be transmitted.
 */
void UART0_Transmit(uint8_t *data);

void UART0_Transmit_Char(char val);

char UART0_Receive_Byte(void);

#endif // UART_H
