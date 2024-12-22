/******************************************************************************
 * Module: UART
 *
 * File Name: UART.c
 *
 * Description: Source file for UART initialization and control functions.
 *
 * Author: El-Osood El-Mefrhda
 ******************************************************************************/

#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/UART.h"

/*******************************************************************************
 *                              Variables                                     *
 *******************************************************************************/

unsigned char UART_Mailbox;         // To store the received data
unsigned short UART_Mail = 0;       // Mail flag (1 = new data available, 0 = no new data)

/*******************************************************************************
 *                           Functions                                       *
 *******************************************************************************/

/**
 * @brief Initializes the UART for 19200 baud rate, 8-bit word length, no parity bits, one stop bit, and FIFOs enabled.
 * @return None
 */
void UART_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;            // Activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;            // Activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;                 // Disable UART while configuring
  UART0_IBRD_R = 260;                              // IBRD = int(80,000,000 / (16 * 19200)) = int(260.402778)
  UART0_FBRD_R = 27;                               // FBRD = round(0.416666 * 64 + 0.5) = 27
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN); // 8-bit word length, no parity bits, one stop bit, FIFOs
  UART0_CTL_R |= UART_CTL_UARTEN;                  // Enable UART

  GPIO_PORTA_AFSEL_R |= 0x03;                      // Enable alt function on PA1, PA0
  GPIO_PORTA_DEN_R |= 0x03;                        // Enable digital I/O on PA1, PA0
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;  // Configure PA1, PA0 as UART0
  GPIO_PORTA_AMSEL_R &= ~0x03;                     // Disable analog functionality on PA1, PA0
	/*
  UART0_IM_R |= UART_IM_RXIM;                      // Enable RX interrupt
  NVIC_EN0_R |= 1 << 5;                            // Enable interrupt 5 in NVIC (UART0)
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xffff00ff) | 0x00008000;  // Set the UART interrupt priority to 4
	*/
}

/**
 * @brief UART0 interrupt handler.
 *        Reads received data, outputs it, and sets the mail flag.
 * @return None
 *//*
void UART0_Handler(void) {
  UART_Mailbox = ((unsigned char)(UART0_DR_R & 0xFF)); // Read received data
  UART_OutChar(UART_Mailbox);                          // Echo the received character
  UART_Mail = 1;                                      // Set mail flag to indicate new data
  UART0_ICR_R = UART_ICR_RXIC;                        // Clear RX interrupt flag
}*/

/**
 * @brief Waits for new input from the serial port.
 * @return ASCII code for the received character.
 */
unsigned char UART_InChar(void){
  while((UART0_FR_R & UART_FR_RXFE) != 0);  // Wait for RXFE flag to be cleared
  return (unsigned char)(UART0_DR_R & 0xFF); // Read and return received character
}

/**
 * @brief Non-blocking function to get the oldest serial port input.
 *        If no data is available, returns 0.
 * @return ASCII code for the received character or 0 if no data.
 */
unsigned char UART_InCharNonBlocking(void){
  if((UART0_FR_R & UART_FR_RXFE) == 0) {
    return (unsigned char)(UART0_DR_R & 0xFF); // Return received character
  } else {
    return 0; // No data available
  }
}

/**
 * @brief Outputs an 8-bit character to the serial port.
 * @param data: 8-bit ASCII character to be transmitted.
 * @return None
 */
void UART_OutChar(unsigned char data){
  while((UART0_FR_R & UART_FR_TXFF) != 0);  // Wait until TX FIFO is not full
  UART0_DR_R = data;                         // Send the data
}

/**
 * @brief Accepts ASCII input in unsigned decimal format and converts it to a 32-bit unsigned number.
 *        Backspace allows removal of last digit typed.
 * @return 32-bit unsigned number
 */
unsigned long UART_InUDec(void){
  unsigned long number = 0, length = 0;
  char character;
  character = UART_InChar();
  while(character != CR) { // Accept input until <enter> is pressed
    if((character >= '0') && (character <= '9')) {
      number = 10 * number + (character - '0'); // Build the number
      length++;
      UART_OutChar(character);  // Echo the character
    }
    else if((character == BS) && length) {  // Handle backspace
      number /= 10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();  // Read the next character
  }
  return number;
}

/**
 * @brief Outputs a null-terminated string to the serial port.
 * @param buffer: Pointer to the string to be transferred.
 * @return None
 */
void UART_OutString(unsigned char buffer[]){
  while(*buffer) {  // Loop through each character
    UART_OutChar(*buffer);  // Output character
    buffer++;  // Move to the next character
  }
}

unsigned char String[10];

/**
 * @brief Converts a 32-bit unsigned number into a fixed 4-digit format and stores the result in a global string.
 *        If the value is greater than 9999, store "****".
 * @param n: 32-bit number to be converted.
 * @return None
 */
void UART_ConvertUDec(unsigned long n) {
  // Conversion logic needs to be implemented
}

/**
 * @brief Outputs a 32-bit unsigned number in a fixed 4-digit decimal format.
 * @param n: Number to be transferred.
 * @return None
 */
void UART_OutUDec(unsigned long n){
  UART_ConvertUDec(n);   // Convert the number
  UART_OutString(String); // Output the converted string
}

/**
 * @brief Converts a 32-bit distance into an ASCII string in the format of 1 digit, a decimal point, 3 digits, and " cm".
 * @param n: Distance in units of 0.001 cm to be converted.
 * @return None
 */
void UART_ConvertDistance(unsigned long n) {
  // Conversion logic needs to be implemented
}

/**
 * @brief Outputs a 32-bit number as a fixed-point distance in the format of 1 digit, a decimal point, 3 digits, and " cm".
 * @param n: Distance in units of 0.001 cm to be transferred.
 * @return None
 */
void UART_OutDistance(unsigned long n){
  UART_ConvertDistance(n);   // Convert the distance
  UART_OutString(String);    // Output the converted string
}
