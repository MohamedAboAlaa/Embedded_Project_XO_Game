//const char sinWave[16] = {7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0};
const unsigned char sinWave[16] = {255,0,255,0,255,0,255,0,255,0,255,0,255,0,255,0};
unsigned volatile char Index = 0, turn_Mark; 
int flag = 0;
// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "../Drivers_Header/Sound.h"
#include "../Drivers_Header/DAC.h"
#include "../Drivers_Header/tm4c123gh6pm.h"

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none



void Sound_Init(unsigned long period, unsigned char turn_mark){
  DAC_Init();
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period -1;
	turn_Mark = turn_mark;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x0007;
	
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period , unsigned char turn_mark, unsigned int FlagDraw){
// this routine sets the RELOAD and starts SysTick
	flag = FlagDraw ;
	turn_Mark = turn_mark;
	NVIC_ST_RELOAD_R = period;
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
	NVIC_ST_CTRL_R = 0;
 // this routine stops the sound output
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	Index = (Index + 1)%(16);
	DAC_Out(sinWave[Index],turn_Mark,flag);
}
