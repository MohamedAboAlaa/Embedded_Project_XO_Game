/******************************************************************************
 * Module: ADC
 *
 * File Name: ADC.h
 *
 * Description: Header file for ADC initialization and functions.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             * 
 *******************************************************************************/ 

#ifndef ADC_H_
#define ADC_H_

/*******************************************************************************
 *                       Functions                             *
 *******************************************************************************/

/**
 * @brief Initializes ADC0 to use sample sequencer 3 for software-triggered conversions.
 *        Configured for Ain1 (PE2) as input source.
 * @param None
 * @return None
 */
void ADC0_InitSWTriggerSeq3_Ch0(void);

#endif /* ADC_H_ */
