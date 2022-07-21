// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/2/2021  
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Labs 8 and 9 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ // Book has setup
	
// write this
//Start Here
	SYSCTL_RCGCADC_R |= 0x01;
	SYSCTL_RCGCGPIO_R |= 0x08;
	
	while((SYSCTL_PRGPIO_R & 0x08) != 0x08){} // wait
	GPIO_PORTD_DIR_R &= ~ 0x04;
	GPIO_PORTD_AFSEL_R |= 0x04;
	GPIO_PORTD_DEN_R &= ~ 0x04;
	GPIO_PORTD_AMSEL_R |= 0x04;
		
	
	ADC0_PC_R = 0x01;  					// 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
		
	ADC0_SSMUX3_R = 5;
		/////////////////////
	ADC0_SSCTL3_R = 0x0006;
		///////////////////
	ADC0_IM_R &=~ 0x0008;
	ADC0_ACTSS_R |= 0x0008;
	//ADC0_SAC_R = 6;
//End Here
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){ // Book has setup
	uint32_t result;
	
	// 1) initiate SS3
	ADC0_PSSI_R = 0x0008;
  // 2) wait for conversion done
	while((ADC0_RIS_R&0x08)==0){};
  // 3) read result
	result = ADC0_SSFIFO3_R&0xFFF;
  // 4) acknowledge completion
	ADC0_ISC_R = 0x0008;
  return result;
}

