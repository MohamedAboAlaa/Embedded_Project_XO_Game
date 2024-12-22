/******************************************************************************
 * Module: GPIO
 *
 * File Name: GPIO.c
 *
 * Description: Source file for GPIO module and its functions
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             * 
 *******************************************************************************/ 

#include "../Drivers_Header/GPIO.h"

/*******************************************************************************
 *                       Variables                             *
 *******************************************************************************/

volatile unsigned long delay;
unsigned char Sw1Flag = 0, Sw2Flag = 0;

/*******************************************************************************
 *                          Functions                             *
 *******************************************************************************/

/**
 * @brief Initializes Port B by enabling the clock, setting pins as outputs, 
 *        and configuring digital functionality.
 * @param None
 * @return None
 */
void PortB_Init(void)
{
    SYSCTL_RCGC2_R |= (1 << 1);                // Enable B clock
    delay = SYSCTL_RCGCGPIO_R;                 // Delay for stabilization
    GPIO_PORTB_DIR_R |= ((1 << 1) | (1 << 2) | (1 << 3)); // PB1 to PB3 as output
    GPIO_PORTB_AFSEL_R = 0x0;                  // No alternate function
    GPIO_PORTB_PCTL_R = 0x00000000;            // Configure as GPIO
    GPIO_PORTB_AMSEL_R = 0x0;                  // Disable analog
    GPIO_PORTB_DEN_R |= ((1 << 1) | (1 << 2) | (1 << 3)); // Enable digital pins
}
/**
 * @brief Initializes Port D by enabling the clock, setting pins as outputs, 
 *        and configuring digital functionality.
 * @param None
 * @return None
 */
void PortD_Init(void)
{
    SYSCTL_RCGC2_R |= (1 << 3);                // Enable D clock
    delay = SYSCTL_RCGCGPIO_R;                 // Delay for stabilization
    GPIO_PORTD_DIR_R |= ((1 << 1) | (1 << 2) | (1 << 3)); // PD1 to PD3 as output
    GPIO_PORTD_AFSEL_R = 0x0;                  // No alternate function
    GPIO_PORTD_PCTL_R = 0x00000000;            // Configure as GPIO
    GPIO_PORTD_AMSEL_R = 0x0;                  // Disable analog
    GPIO_PORTD_DEN_R |= ((1 << 1) | (1 << 2) | (1 << 3)); // Enable digital pins
}

/**
 * @brief Initializes Port F for digital input and output functionality, 
 *        enables pull-up resistors, and configures interrupts.
 * @param None
 * @return None
 */
void PortF_Init(void)
{
    SYSCTL_RCGC2_R |= (1 << 5);                // Enable F clock
    delay = SYSCTL_RCGC2_R;                    // Delay
    GPIO_PORTF_LOCK_R = 0x4C4F434B;            // Unlock Port F
    GPIO_PORTF_CR_R = 0x1F;                    // Allow changes to PF0 to PF4
    GPIO_PORTF_AMSEL_R = 0x0;                  // Disable analog function
    GPIO_PORTF_PCTL_R = 0x0;                   // Configure as GPIO
    GPIO_PORTF_DIR_R |= ((1 << 1) | (1 << 2) | (1 << 3)); // PF1 to PF3 output
    GPIO_PORTF_AFSEL_R = 0x00;                 // No alternate function
    GPIO_PORTF_PUR_R |= ((1 << 0) | (1 << 4)); // Enable pull-up on PF0, PF4
    GPIO_PORTF_DEN_R = 0x1F;                   // Enable digital pins PF0 to PF4
    GPIO_PORTF_IS_R &= ~((1 << 0) | (1 << 4)); // Edge-sensitive for PF0, PF4
    GPIO_PORTF_IBE_R &= ~((1 << 0) | (1 << 4)); // Single edge trigger
    GPIO_PORTF_IEV_R &= ~((1 << 0) | (1 << 4)); // Falling edge trigger
    GPIO_PORTF_ICR_R = ((1 << 0) | (1 << 4));  // Clear interrupt flags
    GPIO_PORTF_IM_R |= ((1 << 0) | (1 << 4));  // Enable interrupts on PF0, PF4
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // Set priority (5)
    NVIC_EN0_R |= (1 << 30);                   // Enable interrupts for Port F
    EnableInterrupts();
}

/**
 * @brief Handles GPIO Port F interrupts for switches SW1 and SW2.
 *        Debounces the switches and sets corresponding flags.
 * @param None
 * @return None
 */
void GPIOPortF_Handler(void)
{
    uint8_t interruptSource = GPIO_PORTF_MIS_R & 0x11; // Mask PF0 and PF4
    GPIO_PORTF_ICR_R = interruptSource;                // Clear interrupt flags

    if (interruptSource & 0x10) {
        Timer2_delay_ms(3);                            // Debounce SW1
        if (!(GPIO_PORTF_DATA_R & 0x10)) {
            Sw1Flag = 1;                               // Set SW1 flag
        }
    }
    if (interruptSource & 0x01) {
        Timer2_delay_ms(3);                            // Debounce SW2
        if (!(GPIO_PORTF_DATA_R & 0x01)) {
            Sw2Flag = 1;                               // Set SW2 flag
        }
    }
}
