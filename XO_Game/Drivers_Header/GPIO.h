/******************************************************************************
 * Module: GPIO
 *
 * File Name: GPIO.h
 *
 * Description: Header file for GPIO initialization functions.
 *              
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

/*******************************************************************************
 *                      Included Libraries                                     *
 *******************************************************************************/
#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/Timer.h"
#include <stdint.h>

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/**
 * @brief Initializes Port A for GPIO functionality.
 *        Configures the pins of Port A as input/output based on specific
 *        requirements for the application.
 *
 * @Note  The all Ports_Init_Functions are with the same purpose as @PortA.
 */
void PortA_Init(void);

void PortB_Init(void);

void PortC_Init(void);

void PortD_Init(void);

void PortE_Init(void);

void PortF_Init(void);

/**
 * @brief Handles the interrupt generated by PortF.
 */
void GPIOPortF_Handler(void);

/**
 * @brief Enables global interrupts on the microcontroller.
 *        This function should be called after all interrupt settings are 
 *        configured to allow the microcontroller to respond to interrupts.
 */
void EnableInterrupts(void);

#endif /* GPIO_H_ */
