// Lab9.c
// Runs on TM4C123
// Student names: Chieh-An Chen
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/19/2022 

// Analog Input connected to PD2=ADC5
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w Fifo on the receiver end 
//    (we suggest implementing and testing this first)

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"
#include "UART.h"
#include "Fifo.h"
#define PC54                  (*((volatile uint32_t *)0x400060C0)) // bits 5-4
#define PF321                 (*((volatile uint32_t *)0x40025038)) // bits 3-1
// TExaSdisplay logic analyzer shows 7 bits 0,PC5,PC4,PF3,PF2,PF1,0 
void LogicAnalyzerTask(void){
  UART0_DR_R = 0x80|PF321|PC54; // sends at 10kHz
}

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
#define STX 0x02
#define ETX 0x03
#define LF  0x0A
#define CR  0x0D
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm
int32_t TxCounter = 0;


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x20;      // activate port F
  while((SYSCTL_PRGPIO_R&0x20) != 0x20){};
  GPIO_PORTF_DIR_R |=  0x0E;   // output on PF3,2,1 (built-in LED)
  GPIO_PORTF_PUR_R |= 0x10;
  GPIO_PORTF_DEN_R |=  0x1E;   // enable digital I/O on PF
}
// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(uint32_t period){
	NVIC_ST_CTRL_R = 0x0007; // enable SysTick with core clock and interrupts
	NVIC_ST_RELOAD_R = period-1;// reload value
	NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
}

// Get fit from excel and code the convert routine with the constants
// from the curve-fit
uint32_t Convert(uint32_t x){
 // write this (copy from Lab 8)
  return (((1679*x)/4096) + 179);
}


// final main program for bidirectional communication
// Sender sends using SysTick Interrupt, Tx uses busy-wait
// Receiver receives using RX interrrupts
int main(void){  
	//char current;
  DisableInterrupts();
  //PLL_Init();  // simulation
  TExaS_Init(&LogicAnalyzerTask); // real board
  ST7735_InitR(INITR_REDTAB);
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  UART_Init();   // your initialize UART
//Enable SysTick Interrupt by calling SysTick_Init()
  SysTick_Init(80000000/10); // Interrupt at 10Hz
  // other initializations as needed
  EnableInterrupts();
	Fifo_Init();
  ST7735_PlotClear(0,2000); // 0 to 200, 0.01cm
	char currentdata[8];
	uint32_t statusdata[20];
  while(1){ // one time through the loop every 100 ms
// get message from software FIFO
		ST7735_SetCursor(0,0);
		ST7735_OutString("Lab 9, d=   "); 
		ST7735_SetCursor(9,0);
		// output to LCD
		for(int i = 0; i < 8; i++)
		{
			statusdata[i] = Fifo_Get(&currentdata[i]);
		}
		for(int i = 0; i < 8; i++)
		{
			if ((currentdata[i] != 0x3C) && (currentdata[i] != 0x3E) && (currentdata[i] != 0x0A))
			{
				ST7735_OutChar(currentdata[i]);
			}	
		}
		ST7735_SetCursor(14,0);
		ST7735_OutString(" cm    ");
		PF3 ^= 0x08;       // Heartbeat when message received
	}
// output to LCD
}

uint32_t message;
uint32_t array[8];
void SysTick_Handler(void){ // every 100 ms
 //Similar to Lab8 except rather than grab sample and put in mailbox
 //        format message and transmit 
	GPIO_PORTF_DATA_R ^= 0x02; // toggle PF1
	Data = ADC_In();      // Sample ADC
	message = Convert(Data);
	array[0] = 0x3c;
	array[1] = ((message/1000)+0x30);
	message = message%1000;
	array[2] = 0x2E;
	array[3] = ((message/100)+0x30);
	message = message%100;
	array[4] = ((message/10) + 0x30);			//Digit 3
	message = message%10;
	array[5] = (message + 0x30);     //Digit 4
	array[6] = 0x3E; //>
	array[7] = 0x0A; //LF
	for(int i = 0; i < 8; i++){
		UART_OutChar(array[i]);
	}
	TxCounter++;
}


uint32_t Status[20];             // entries 0,7,12,19 should be false, others true
char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int mainfifo(void){ // Make this main to test Fifo
  Fifo_Init(); // Assuming a buffer of size 6
  for(;;){
    Status[0]  = Fifo_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = Fifo_Put(1);            // should succeed, 1 
    Status[2]  = Fifo_Put(2);            // should succeed, 1 2
    Status[3]  = Fifo_Put(3);            // should succeed, 1 2 3
    Status[4]  = Fifo_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = Fifo_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = Fifo_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = Fifo_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = Fifo_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = Fifo_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = Fifo_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = Fifo_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = Fifo_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = Fifo_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = Fifo_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = Fifo_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = Fifo_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = Fifo_Get(&GetData[7]);  // should succeed, 8
    Status[18] = Fifo_Get(&GetData[8]);  // should succeed, empty
    Status[19] = Fifo_Get(&GetData[9]);  // should fail,    empty
  }
}
