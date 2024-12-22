/******************************************************************************
 * Module: ADC
 *
 * File Name: ADCTestMain.c
 *
 * Description: Source file for ADC module initialization and interrupt handling.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             * 
 *******************************************************************************/ 

#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/PLL.h"
#include "../Drivers_Header/ADC.h"

/*******************************************************************************
 *                       Variables                             *
 *******************************************************************************/

volatile unsigned int ADCvalue; // Digital representation of voltage on PE4
extern int F1, F2;

/*******************************************************************************
 *                          Functions                             *
 *******************************************************************************/

/**
 * @brief Initializes ADC0 for software-triggered conversion on channel 0 (PE3).
 *        Sets up GPIO and ADC configurations, and enables interrupts.
 * @param None
 * @return None
 */
void ADC0_InitSWTriggerSeq3_Ch0(void)
{
    volatile unsigned long delay;
    if (F2 == 1) {
        ADCvalue = 0;
    }
    
    // Enable Clock to ADC0 and GPIO pins
    SYSCTL_RCGCGPIO_R |= (1 << 4);    // Enable Clock to GPIOE or PE3/AN0
    SYSCTL_RCGCADC_R |= (1 << 0);     // ADC0 clock enable

    // Initialize PE3 for AIN0 input
    GPIO_PORTE_AFSEL_R |= (1 << 3);   // Enable alternate function
    GPIO_PORTE_DEN_R &= ~(1 << 3);    // Disable digital function
    GPIO_PORTE_AMSEL_R |= (1 << 3);   // Enable analog function

    // Initialize sample sequencer 3
    ADC0_ACTSS_R &= ~(1 << 3);        // Disable SS3 during configuration
    ADC0_EMUX_R &= ~0xF000;           // Software trigger conversion
    ADC0_SSMUX3_R = 0;                // Input from channel 0
    ADC0_SSCTL3_R |= (1 << 1) | (1 << 2); // One sample, set flag at first sample

    // Enable ADC interrupt
    ADC0_IM_R |= (1 << 3);            // Unmask ADC0 sequence 3 interrupt
    NVIC_EN0_R |= 0x00020000;         // Enable IRQ17 for ADC0SS3
    ADC0_ACTSS_R |= (1 << 3);         // Enable ADC0 sequencer 3
    ADC0_PSSI_R |= (1 << 3);          // Start sampling data from AN0
}

/**
 * @brief ADC interrupt handler for sequence 3.
 *        Reads conversion results and clears interrupt flags.
 * @param None
 * @return None
 */
void ADC0Seq3_Handler(void)
{
    if (F1 == 1) {
        ADCvalue = ADC0_SSFIFO3_R;    // Read ADC conversion result from SS3 FIFO
    }
    ADC0_ISC_R |= 0x08;               // Clear conversion complete flag
    ADC0_PSSI_R |= 0x08;              // Start next sampling data from AN0
}
