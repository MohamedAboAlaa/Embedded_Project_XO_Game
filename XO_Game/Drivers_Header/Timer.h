/******************************************************************************
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Description: Header file for Timer module functions and configurations.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

/*******************************************************************************
 *                       Headers                              *
 *******************************************************************************/

#include "../Drivers_Header/tm4c123gh6pm.h"

/*******************************************************************************
 *                      Function Prototypes                                   *
 *******************************************************************************/

/**
 * @brief Generates a delay for a specified period in milliseconds using Timer2.
 * @param period_ms: The delay duration in milliseconds.
 * @return None
 */
void Timer2_delay_ms(unsigned long period_ms);

/**
 * @brief Interrupt handler for Timer2A timeout.
 * @return None
 */
void Timer2A_Handler(void);

#endif // TIMER_H
