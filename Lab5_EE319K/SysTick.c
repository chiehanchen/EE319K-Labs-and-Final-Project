// SysTick.c
// Runs on TM4C123
// Put your names here
// Last Modified: 1/11/2022
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value


// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
    // EE319K students write this function
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;  			// maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock; Clock Source = 1, Interrupts disabled = 0; Enable SysTick = 1; 0101 = 0x5

}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){
  // EE319K students write this function
	uint32_t start = NVIC_ST_CURRENT_R;
	uint32_t elapsed = 0; // Actually calculate how much elapsed time has occured
  do
	{
    elapsed = (start - NVIC_ST_CURRENT_R) & 0x00FFFFFF; // This is a down-counter, how elapsed time is calculated
  }
  while(elapsed <= delay);

}
// 10000us equals 10ms
void SysTick_Wait10ms(uint32_t delay){
    // EE319K students write this function
	uint32_t i;
	for(i = 0; i < delay; i++) {
		SysTick_Wait(800000); // Wait for 10ms
	}

}

