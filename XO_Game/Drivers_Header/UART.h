/******************************************************************************
 * Module: UART
 *
 * File Name: UART.h
 *
 * Description: Header file for UART functionality.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

#ifndef UART_H
#define UART_H

/*******************************************************************************
 *                              Constants                                      *
 *******************************************************************************/

#define CR   0x0D  // Carriage return
#define LF   0x0A  // Line feed
#define BS   0x08  // Backspace
#define ESC  0x1B  // Escape
#define SP   0x20  // Space
#define DEL  0x7F  // Delete

/*******************************************************************************
 *                          Function Prototypes                                *
 *******************************************************************************/

/**
 * @brief Initializes the UART for 115200 baud rate, 8-bit word length, no parity, one stop bit, and FIFOs enabled.
 * @return None.
 */
void UART_Init(void);

/**
 * @brief Waits for new input from the serial port.
 * @return ASCII code for the received character.
 */
unsigned char UART_InChar(void);

/**
 * @brief Non-blocking function to get the oldest serial port input.
 * @return ASCII code for the received character, or 0 if no data is available.
 */
unsigned char UART_InCharNonBlocking(void);

/**
 * @brief Outputs an 8-bit character to the serial port.
 * @param data: 8-bit ASCII character to be transmitted.
 * @return None.
 */
void UART_OutChar(unsigned char data);

/**
 * @brief Accepts ASCII input in unsigned decimal format and converts to a 32-bit unsigned number.
 * @return 32-bit unsigned number.
 */
unsigned long UART_InUDec(void);

/**
 * @brief Outputs a null-terminated string to the serial port.
 * @param buffer: Pointer to the string to be transferred.
 * @return None.
 */
void UART_OutString(unsigned char buffer[]);

/**
 * @brief Converts a 32-bit unsigned number into a fixed 4-digit format.
 *        Stores the result in a global string.
 * @param n: Number to be converted.
 * @return None.
 */
void UART_ConvertUDec(unsigned long n);

/**
 * @brief Outputs a 32-bit number in unsigned decimal format with fixed 4 digits.
 * @param n: Number to be transferred.
 * @return None.
 */
void UART_OutUDec(unsigned long n);

/**
 * @brief Converts a 32-bit distance into an ASCII string in the format of 1 digit, a decimal point, 3 digits, and " cm".
 * @param n: Distance in units of 0.001 cm to be converted.
 * @return None.
 */
void UART_ConvertDistance(unsigned long n);

/**
 * @brief Outputs a 32-bit number as a fixed-point distance in the format of 1 digit, a decimal point, 3 digits, and " cm".
 * @param n: Distance in units of 0.001 cm to be transferred.
 * @return None.
 */
void UART_OutDistance(unsigned long n);

#endif // UART_H
