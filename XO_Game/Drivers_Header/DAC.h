/******************************************************************************
 * Module: DAC
 *
 * File Name: DAC.h
 *
 * Description: Header file for DAC functionality.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data, unsigned char turnMark,unsigned int FlagDraw);
  


