// Key.c
// This software configures the off-board piano keys
// Lab 6 requires a minimum of 4 keys, but you could have more
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/11/22  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: none
void Key_Init(void){ volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x10;     // 1) activate clock for Port E
	
	__asm__ {
		NOP
		NOP
	}

	GPIO_PORTE_DIR_R &= ~(0x0F);		// Sets direction of PE3, PE2, PE1, PE0; these are the switch inputs
	GPIO_PORTE_DEN_R |= 0x0F;		// Digitally enables PE3, PE2, PE1, PE0; these are the switch inputs
}
// **************Key_In*********************
// Input from piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
	// Read and Mask the input keys
	return (GPIO_PORTE_DATA_R & 0x0F);
  
}

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
// implement if needed
	SYSCTL_RCGCGPIO_R |= 0x020; // heartbeat clock F on
	while((SYSCTL_RCGCGPIO_R & 0x20) == 0){ 
		// stabilize
	}
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_DIR_R = 0x0F; // DIR = 1 means output
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R = 0x1F;
	GPIO_PORTF_DATA_R = 0;
}


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
// implement if needed
	uint32_t inputData;
	inputData = ~ GPIO_PORTA_DATA_R;
  return ((inputData & 0x01) | (inputData & 0x02));
   
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of PORTF
// implement if needed
	GPIO_PORTF_DATA_R = data << 3;
}

