// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "../Drivers_Header/DAC.h"
#include "../Drivers_Header/tm4c123gh6pm.h"
//#define PD3_0 (*((volatile unsigned long *)0x4000503C))

unsigned char val = 0,out = 0;	
// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none



void DAC_Init(void){
	
	//  just init the  port what we will send to out
	//PortF_Init();
}


//  we closed Amsel as we try to make it DAC to pwm

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data, unsigned char turnMark,unsigned int FlagDraw){
	//PD3_0 = data;
	out  = data;
	// Draw
	if(FlagDraw){
		if(out){
			//val = 1<<data ;
			GPIO_PORTD_DATA_R = 0x08;	
		}
		else{
			GPIO_PORTD_DATA_R = 0;	
		}
		return;
	}
	if(turnMark == 'X'){
		if(out)
	{
		//val = 1<<data ;
		GPIO_PORTD_DATA_R = 0x02;	
	}
	else
	{
		GPIO_PORTD_DATA_R = 0;	
	}
	
	}
		else  if(turnMark == 'O'){
				
				if(out)
	{
		//val = 1<<data ;
		GPIO_PORTD_DATA_R = 0x04;	
	}
	else
	{
		GPIO_PORTD_DATA_R = 0;	
	}
		
		}
}
