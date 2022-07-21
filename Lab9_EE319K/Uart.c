// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  
// Daniel Valvano
// November 11, 2021

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART.h"
#include "../inc/tm4c123gh6pm.h"
#define LF  0x0A

int RxCounter=0;
// Initialize UART1 on PC4 PC5
// Baud rate is 1000 bits/sec
// Receive interrupts and FIFOs are used on PC4
// Transmit busy-wait is used on PC5.
void UART_Init(void){
  // write this
	
//Start Here
	
/*
	//More from book
	SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1

  SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C

  UART1_CTL_R &= ~0x00000001;    // disable UART

  UART1_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)

  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26

  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs

  UART1_CTL_R |= 0x00000001;     // enable UART

  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4

  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1

  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;

  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
*/
	Fifo_Init();
	SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
	SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
	while((SYSCTL_RCGCUART_R&0x02) == 0){}; // Busy Wait
	UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 5000;     // IBRD = int(80,000,000/(16*1000)) = int(5000)
  UART1_FBRD_R = 0;     // FBRD = round(0 * 64) = 0 -> fractional bits
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, enable FIFO
	UART1_CTL_R |= 0x00000001;     // enable UART
	
	GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // enable PC5-4
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4

	UART1_IM_R |= 0x10;
	UART1_IFLS_R = (UART1_IFLS_R&~0x28)+0x10;
		
	NVIC_PRI1_R = 6;
	NVIC_EN0_R = 0x40;
	
	
/* From Book
	int delay;
	//FiFo_Init();
	SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
	delay=0x5000;
		while(delay){
		delay--;
	}
  SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
	delay=0x5000;
		while(delay){
		delay--;
	}
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 50;     // IBRD = int(80,000,000/(16*100,000)) = int(50)
  UART1_FBRD_R = 0;     // FBRD = round(0 * 64) = 0
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
	UART1_IFLS_R = UART1_IFLS_R&(~0x38)+0x10;
	UART1_IM_R |= 0x10;
  UART1_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF0FFFFF) | 0x00600000;
	NVIC_EN0_R = 0x40;
	
*/
//End Here
	
}

//------------UART_InChar------------
// Receive new input, interrupt driven
// Input: none
// Output: return read ASCII code from UART, 
// Reads from software FIFO (not hardware)
// Either return 0 if no data or wait for data (your choice)
unsigned char UART1_InChar(void){
  // write this
  
//Start Here
	// More from Book
	// Wait for new input, then return ASCII code
	while((UART1_FR_R&0x0010) != 0){};      // BUSY wait until RXFE is 0

  return((char)(UART1_DR_R&0xFF));
	
/*
	while((UART1_FR_R&0x0010) != 0){}; // wait until RXFE is 0
  return((uint8_t)(UART1_DR_R&0xFF));
*/
	
//End Here
	
	//return 0;
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until LR is received
//    or until max length of the string is reached.
// Reads from software FIFO (not hardware)
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
// optional implement this here or in Lab 9 main
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
// busy-wait; if TXFF is 1, then wait
// Transmit busy-wait is used on PC5.
void UART_OutChar(char data){
  // write this
	
//Start Here
	// Wait for buffer to be not full, then output
	while((UART1_FR_R&0x0020) != 0){};      // wait until TXFF is 0

  UART1_DR_R = data;
/*
	while((UART1_FR_R&0x0020) != 0){};      // wait until TXFF is 0
  UART1_DR_R = data;
*/
	
//End Here

}
#define PF2       (*((volatile uint32_t *)0x40025010))
// hardware RX FIFO goes from 7 to 8 or more items
// Receive interrupts and FIFOs are used on PC4
void UART1_Handler(void){
  PF2 ^= 0x04;  // Heartbeat
// write this
	
//Start Here
	
	uint32_t RxCounter = 0; // Initialize RXCounter
	uint32_t RXFECheck = 0;
	while((UART1_FR_R & UART_FR_RXFE) == 0){
		Fifo_Put(UART1_DR_R);
	}
	RxCounter++;
	UART1_ICR_R = 0x10;   // this clears bit 4 (RXRIS) in the RIS register
	
	
/*
		while((UART1_FR_R & 0x10)==0){
			//FiFo_Put(UART_InChar());
		
	}
	RxCounter++;
	UART1_ICR_R = 0x10;   // this clears bit 4 (RXRIS) in the RIS register
	//GPIO_PORTF_DATA_R^=0x04; // This is simply heartbeat on PF2
*/
//End Here	
	
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  // if needed
  while(*pt){
		UART_OutChar(*pt);
		pt++;
	}
}
