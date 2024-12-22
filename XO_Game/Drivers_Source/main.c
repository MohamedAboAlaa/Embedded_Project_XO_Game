/******************************************************************************
 * Module: Main File
 *
 * File Name: Main.c
 *
 * Description: Main file for initializing peripherals and running the XO game.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             * 
 *******************************************************************************/ 

// ineer
#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/Nokia5110.h"
#include "../Drivers_Header/TExaS.h"
#include "../Drivers_Header/GPIO.h"
#include "../Drivers_Header/Game.h"
#include "../Drivers_Header/PLL.h"
#include "../Drivers_Header/ADC.h"
#include "../Drivers_Header/UART.h"
#include "../Drivers_Header/Timer.h"

/*******************************************************************************
 *                          Functions                             *
 *******************************************************************************/

/**
 * @brief Main function that initializes all modules and runs the XO game.
 * @param None
 * @return int: Exit status of the program (default is 0)
 */
int main(void)
{
    TExaS_Init(SSI0_Real_Nokia5110_Scope); // Initialize scope for debugging

    ADC0_InitSWTriggerSeq3_Ch0();          // Initialize ADC for PE3 input

    PortF_Init();                          // Initialize Port F

    PortB_Init();                          // Initialize Port B

		PortD_Init();                          // Initialize Port D
	
    UART_Init();                           // Initialize UART

    Nokia5110_Init();                      // Initialize Nokia LCD

    Nokia5110_ClearBuffer();               // Clear Nokia LCD buffer

    Nokia5110_DisplayBuffer();             // Display initial buffer

    GameIntro();                           // Display game introduction

    PlayXO();                              // Start the XO game

    return 0;                              // Program exit status
}
