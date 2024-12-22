/******************************************************************************
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: Source file for Timer module functions and configurations.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             *
 *******************************************************************************/

#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/Timer.h"

/*******************************************************************************
 *                       Variables                             *
 *******************************************************************************/

unsigned long Semaphore = 0;

/*******************************************************************************
 *                          Functions                             *
 *******************************************************************************/

/**
 * @brief Delays for a specified period in milliseconds using Timer2.
 * @param period_ms: The delay duration in milliseconds.
 * @return None
 */
void Timer2_delay_ms(unsigned long period_ms) {
    volatile unsigned long delay;

    SYSCTL_RCGCTIMER_R |= (1 << 2);                     // Activate Timer2
    delay = SYSCTL_RCGCTIMER_R;                         // Allow clock to stabilize

    TIMER2_CTL_R &= ~(1 << 0);                          // Disable Timer2A during setup
    TIMER2_CFG_R = 0x00;                                // Configure for 32-bit timer mode
    TIMER2_TAMR_R = 0x02;                               // Configure for periodic mode

    TIMER2_TAILR_R = (80000 * period_ms) - 1;           // Set reload value for the given period in ms
    TIMER2_ICR_R = 0x01;                                // Clear timer timeout flag
    TIMER2_IMR_R = 0x01;                                // Arm timeout interrupt

    NVIC_PRI5_R = (NVIC_PRI5_R & 0x00FFFFFF) | 0x80000000;  // Priority 4
    NVIC_EN0_R = 1 << 23;                               // Enable IRQ 23 in NVIC
    TIMER2_CTL_R |= 0x01;                               // Enable Timer2A
    Semaphore = 0;
    while (Semaphore == 0);
}

/**
 * @brief Interrupt handler for Timer2A timeout event.
 * @param None
 * @return None
 */
void Timer2A_Handler(void) {
    TIMER2_ICR_R = 0x01;                                // Acknowledge Timer2A timeout
    Semaphore = 1;                                      // Signal timeout
}
