// ConnectFour.c
// Runs with Logo starter file
// Jonathan Valvano
// November 29, 2012
// Commented by Ramesh Yerraballi 4/15/13


/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include "hw_types.h"
#include "sysctl.h"
#include "lm3s1968.h"
#include "rit128x96x4.h"
#include "logo.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif
// 16-color BMP file format
// index 18-21 little endian is the width
// index 22-25 little endian is the height
// starting with index 0x76 and going to the second to last byte is the matrix data 4 bits/pixel
// the width is not a multiple of 16, then padding occurs so the number of bytes in each row is divisible by 8

const unsigned char Logo3[] ={
 0x42, 0x4D, 0xEE, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
 0x72, 0x00, 0x00, 0x00,   // width 0x72 = 114
 0x42, 0x00, 0x00, 0x00,   // height 0x42 = 66
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
 0x05, 0x57, 0x77, 0x77, 0x77, 0x77, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x05, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x44, 0x57, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x05, 0x57, 0x77, 0x77, 0x55, 0x00, 0x05, 0x45, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x55, 0x55, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x11, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x57, 0x77,
 0x79, 0x77, 0x77, 0x77, 0x77, 0x77, 0x11, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x57, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x77, 0x77, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x74, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x05, 0x55, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x45, 0x57,
 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x77, 0x55, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x57, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x55, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x47, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x8F, 0x88, 0x88, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x77, 0x77, 0x77, 0x75, 0x54, 0x47, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
 0x77, 0x77, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xFF, 0xFF, 0xF8, 0x00, 0x06, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x54, 0x74, 0x00, 0x00, 0x44, 0x47, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x70, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x05, 0x50, 0x07, 0x77, 0x97, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x15, 0x57, 0x55, 0x00, 0x00, 0x00,
 0x06, 0x70, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x07, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x10, 0x00, 0x54, 0x77, 0x77, 0x77,
 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x05, 0x17, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x88, 0x88, 0x88, 0x77, 0x77, 0x75, 0x00, 0x57, 0x88, 0x88, 0x77, 0x77, 0x88, 0x88, 0x88,
 0x88, 0xF8, 0xF8, 0x00, 0x01, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x75, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x08, 0xF8, 0x8F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0xFF, 0xFF, 0xFF, 0x88, 0x87, 0x77, 0x10, 0x58, 0xFF, 0xFF, 0x88, 0x8F, 0xFF, 0xFF, 0x88, 0xF8, 0xF8, 0xF8, 0x00,
 0x05, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
 0xF8, 0x88, 0x88, 0x00, 0x00, 0x00, 0x50, 0x78, 0xFF, 0x88, 0x88, 0x88, 0x88, 0x88, 0x77, 0x71, 0x8F, 0x8F, 0x88, 0xFF, 0x88, 0x88, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x00, 0x00, 0x05, 0x77, 0x77,
 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x8F, 0xF8, 0x88,
 0x88, 0x80, 0x00, 0x7F, 0xF8, 0xFF, 0xFF, 0xF8, 0x88, 0xFF, 0x87, 0x77, 0xF8, 0xFF, 0x88, 0x8F, 0x88, 0x8F, 0x8F, 0x88, 0xF8, 0xF8, 0xF8, 0x00, 0x06, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x50,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0xFF, 0x88, 0x8F, 0x80, 0x07, 0xF8,
 0x8F, 0x8F, 0xF8, 0xF8, 0x8F, 0xF8, 0x88, 0x78, 0xF8, 0xF8, 0x8F, 0xF8, 0xF8, 0x88, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x05, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0x8F, 0x80, 0x07, 0xFF, 0x88, 0x88, 0xF8, 0xFF,
 0x88, 0x88, 0x88, 0x78, 0xF8, 0x88, 0xFF, 0x88, 0xFF, 0x88, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x8F, 0xFF, 0x88, 0x88, 0x80, 0x08, 0xF8, 0xFF, 0x88, 0x87, 0x78, 0xF8, 0xF8, 0x8F, 0x78,
 0xF8, 0x8F, 0xF8, 0x00, 0x8F, 0xF8, 0x8F, 0x88, 0xF8, 0x8F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x8F, 0x88, 0x8F, 0x80, 0x0F, 0x88, 0x88, 0xF8, 0x70, 0x07, 0xFF, 0x88, 0x8F, 0x88, 0xFF, 0x88, 0xF8, 0x05,
 0x8F, 0xF8, 0x8F, 0x88, 0xF8, 0x8F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x8F, 0x88, 0xFF, 0x00, 0x0F, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x8F, 0x88, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x00, 0x8F, 0xF8, 0x8F, 0x88,
 0xF8, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x08, 0xF8, 0x8F, 0xF8, 0x88, 0x88, 0x00, 0x0F, 0x88, 0x88, 0x88, 0x77, 0x77, 0x8F, 0xF8, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x05, 0x0F, 0xF8, 0xFF, 0x88, 0xF8, 0x8F, 0x88, 0x80,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
 0xF8, 0x88, 0x88, 0x00, 0x00, 0x00, 0x08, 0xFF, 0x88, 0x88, 0x77, 0x78, 0x88, 0xFF, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x00, 0x0F, 0xF8, 0xFF, 0x88, 0xF8, 0x8F, 0xFF, 0xF8, 0x80, 0x00, 0x00, 0x00,
 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x88, 0x08,
 0x08, 0x80, 0x88, 0xFF, 0x8F, 0xFF, 0x88, 0x88, 0x8F, 0x8F, 0x8F, 0x88, 0xF8, 0x88, 0xF8, 0x00, 0x0F, 0xF8, 0xFF, 0x88, 0xF8, 0x88, 0x88, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0xF8, 0x8F, 0xFF, 0xF8, 0xF8, 0xF8, 0xFF,
 0xFF, 0xF8, 0x8F, 0xF8, 0x88, 0x88, 0x8F, 0x78, 0xF8, 0x88, 0xF8, 0x00, 0x0F, 0x88, 0x88, 0x87, 0xF8, 0x88, 0xFF, 0x8F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x8F, 0x88, 0xF8, 0x8F, 0xFF, 0xF8, 0x8F,
 0xF8, 0x88, 0xF8, 0x58, 0xFF, 0xF8, 0x88, 0x00, 0x0F, 0x8F, 0xFF, 0x88, 0xF8, 0xF8, 0x88, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x40, 0x00, 0x44, 0x00, 0x48, 0x88, 0xF8, 0x88, 0xF8, 0x88, 0x8F, 0x87, 0x7F, 0x88, 0x88, 0xFF, 0x8F, 0x88, 0x88, 0x87, 0x48,
 0xFF, 0x88, 0xF8, 0x00, 0x8F, 0xFF, 0xFF, 0x87, 0xF8, 0xF8, 0xF8, 0x8F, 0xF8, 0x04, 0x00, 0x00, 0x70, 0x40, 0x00, 0x00, 0x44, 0x54, 0x44, 0x45, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x46, 0x44, 0x48, 0xFF, 0x8F, 0x88, 0xF8, 0x8F, 0xFF, 0x87, 0x17, 0xFF, 0x88, 0x88, 0x88, 0x8F, 0x88, 0x70, 0x08, 0xF8, 0xFF, 0xF8, 0x10,
 0x0F, 0xFF, 0xFF, 0x87, 0xFF, 0x88, 0xF8, 0x78, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x05, 0x55, 0x50, 0x05, 0x77, 0x77, 0x77, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55,
 0x77, 0x77, 0x77, 0x75, 0x05, 0x55, 0x55, 0x58, 0xFF, 0x8F, 0xFF, 0xFF, 0x88, 0xFF, 0x70, 0x00, 0x78, 0x8F, 0x88, 0xF8, 0x88, 0x05, 0x00, 0x00, 0xF8, 0x88, 0xF8, 0x10, 0x08, 0x88, 0x88, 0x85,
 0x88, 0x88, 0x87, 0x00, 0x77, 0x00, 0x00, 0x00, 0x57, 0x77, 0x77, 0x75, 0x07, 0x77, 0x88, 0x87, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x78, 0x88, 0x77, 0x77, 0x77,
 0x15, 0x57, 0x77, 0x78, 0x88, 0x88, 0x88, 0x77, 0x88, 0x88, 0x77, 0x77, 0x00, 0x78, 0x88, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x44, 0x45, 0x77, 0x77, 0x77, 0x77,
 0x50, 0x00, 0x00, 0x07, 0x88, 0x88, 0x88, 0x71, 0x18, 0x88, 0xFF, 0xF8, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0xFF, 0x88, 0xF8, 0x88, 0x77, 0x01, 0x78, 0x88, 0x88,
 0x87, 0x77, 0x75, 0x00, 0x77, 0x88, 0x88, 0x77, 0x07, 0x77, 0x78, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0x78, 0x88, 0x88, 0x00, 0x04, 0x57, 0x78, 0x88, 0x88, 0x87, 0x77, 0x80, 0x05, 0x47, 0x88,
 0xF8, 0x88, 0xF8, 0x77, 0x8F, 0xF8, 0x88, 0x88, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xF8, 0x88, 0x88, 0xF8, 0x77, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x88, 0x77, 0x71,
 0x8F, 0xFF, 0x88, 0x87, 0x08, 0xF8, 0x88, 0xF8, 0x8F, 0xFF, 0x8F, 0xF0, 0x00, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x78, 0xFF, 0xFF, 0xFF, 0x88, 0x88, 0x00, 0x00, 0x78, 0xFF, 0x88, 0xF8, 0xF8, 0x77,
 0xFF, 0x8F, 0x88, 0xF8, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x8F, 0xF8, 0x8F, 0x88, 0x8F, 0x77, 0x7F, 0x88, 0x88, 0x88, 0x88, 0xFF, 0x87, 0x77, 0x88, 0x8F, 0x8F, 0x87,
 0x08, 0xF8, 0x88, 0xF8, 0x8F, 0xFF, 0xF8, 0x88, 0x00, 0xFF, 0x8F, 0xFF, 0x80, 0x07, 0x8F, 0x88, 0xF8, 0x8F, 0xFF, 0xFF, 0x80, 0x00, 0x8F, 0xF8, 0x88, 0x88, 0xF8, 0x78, 0xF8, 0x88, 0xFF, 0xF8,
 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x88, 0x88, 0x8F, 0x88, 0xF8, 0xF8, 0x78, 0xFF, 0x8F, 0x88, 0xFF, 0xFF, 0x8F, 0x88, 0x77, 0x88, 0x8F, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88,
 0x8F, 0x8F, 0xF8, 0x88, 0x00, 0xFF, 0xFF, 0xFF, 0x80, 0x08, 0xFF, 0x8F, 0xFF, 0xF8, 0x88, 0xF8, 0x80, 0x08, 0xF8, 0x8F, 0x88, 0xF8, 0xF8, 0x77, 0xF8, 0x88, 0xFF, 0xFF, 0x74, 0x40, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x07, 0xFF, 0x88, 0xF8, 0x88, 0x88, 0xF8, 0xFF, 0x88, 0x88, 0x8F, 0x88, 0x88, 0x88, 0x88, 0xFF, 0x77, 0x88, 0x88, 0x8F, 0x87, 0x08, 0xFF, 0x88, 0x88, 0x8F, 0x8F, 0xF8, 0x88,
 0x00, 0xF8, 0xFF, 0xFF, 0x80, 0x8F, 0xFF, 0xFF, 0x88, 0xFF, 0xFF, 0x8F, 0x80, 0x0F, 0xF8, 0xFF, 0xFF, 0xFF, 0xF8, 0x07, 0xFF, 0x88, 0x88, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
 0xF8, 0x88, 0xF8, 0xFF, 0xFF, 0xF8, 0xFF, 0x8F, 0x88, 0xF8, 0xFF, 0xFF, 0x88, 0xFF, 0x8F, 0x87, 0x8F, 0x88, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0x8F, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0xFF,
 0x00, 0xF8, 0x8F, 0xF8, 0xFF, 0x88, 0x8F, 0xFF, 0x00, 0x8F, 0x88, 0xF8, 0xF8, 0x77, 0x77, 0x07, 0xF8, 0x8F, 0x88, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x88, 0x8F, 0x8F, 0xF8,
 0x77, 0x78, 0x8F, 0x88, 0x88, 0x8F, 0xF8, 0x78, 0x8F, 0xFF, 0x8F, 0x87, 0x8F, 0x8F, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0xF8, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0xFF, 0x08, 0xF8, 0x88, 0xFF,
 0xF8, 0x00, 0x08, 0x8F, 0x00, 0x88, 0x8F, 0x8F, 0x87, 0x00, 0x06, 0x07, 0xF8, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x8F, 0xFF, 0xFF, 0x85, 0x44, 0x44, 0x78, 0xF8,
 0xFF, 0xFF, 0x87, 0x00, 0x7F, 0xF8, 0xF8, 0x88, 0x88, 0x8F, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0xF8, 0xF8, 0xF8, 0x00, 0x88, 0x88, 0xFF, 0x08, 0x8F, 0x88, 0xFF, 0xFF, 0xF0, 0x00, 0x08,
 0x01, 0xF8, 0x88, 0x88, 0x87, 0x00, 0x00, 0x07, 0xFF, 0x88, 0x8F, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0xF8, 0xF8, 0x70, 0x40, 0x00, 0x07, 0xFF, 0xF8, 0x88, 0x87, 0x00,
 0x78, 0x88, 0xF8, 0x88, 0x8F, 0x8F, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0x88, 0x88, 0xF8, 0x00, 0x88, 0x88, 0xFF, 0x88, 0xF8, 0x8F, 0xFF, 0x88, 0xFF, 0x80, 0x00, 0x00, 0xFF, 0x88, 0x88,
 0x77, 0x50, 0x00, 0x07, 0xFF, 0x88, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x8F, 0x88, 0x8F, 0xF8, 0x70, 0x50, 0x40, 0x07, 0x8F, 0x88, 0xFF, 0x77, 0x70, 0x78, 0xFF, 0x88, 0xF8,
 0x8F, 0x88, 0x8F, 0x87, 0x08, 0xF8, 0x88, 0x88, 0x8F, 0x88, 0x88, 0xF8, 0x00, 0x88, 0xF8, 0xFF, 0x88, 0xFF, 0x88, 0x8F, 0xFF, 0x88, 0x88, 0x80, 0x08, 0xF8, 0xF8, 0x8F, 0x77, 0x77, 0x77, 0x77,
 0x8F, 0xF8, 0xFF, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x88, 0x8F, 0x87, 0x71, 0x00, 0x00, 0x07, 0xF8, 0x88, 0xFF, 0x87, 0x77, 0x88, 0xFF, 0x8F, 0xF8, 0x8F, 0x88, 0x88, 0x87,
 0x78, 0x88, 0x88, 0x88, 0x7F, 0x8F, 0x88, 0xF0, 0x08, 0x88, 0xF8, 0xFF, 0x88, 0xF8, 0x8F, 0xF8, 0x8F, 0x8F, 0x8F, 0xF8, 0x00, 0xFF, 0xF8, 0x8F, 0x88, 0x77, 0x78, 0x88, 0x8F, 0x8F, 0x8F, 0xF8,
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x88, 0x88, 0x88, 0x87, 0x71, 0x10, 0x00, 0x07, 0x8F, 0xFF, 0xFF, 0x88, 0x88, 0x8F, 0x88, 0xF8, 0xF7, 0xF8, 0xF8, 0xFF, 0x88, 0x88, 0x88, 0xF8, 0x88,
 0x7F, 0x88, 0x8F, 0x88, 0x08, 0xFF, 0x88, 0xFF, 0x88, 0xFF, 0xFF, 0x88, 0x77, 0x88, 0xFF, 0x8F, 0x88, 0x88, 0x8F, 0xF8, 0x8F, 0x88, 0x88, 0x8F, 0x88, 0x88, 0xF8, 0x88, 0x80, 0x50, 0x00, 0x00,
 0x00, 0x00, 0x07, 0x88, 0xF8, 0xF8, 0xF7, 0x77, 0x00, 0x00, 0x00, 0x88, 0xFF, 0x8F, 0xFF, 0xFF, 0xF8, 0x8F, 0x88, 0x87, 0x88, 0x88, 0x88, 0xFF, 0xF8, 0x88, 0xFF, 0x88, 0x8F, 0x88, 0x8F, 0xFF,
 0xFF, 0x88, 0x8F, 0x8F, 0x80, 0xFF, 0xFF, 0xFF, 0x87, 0x78, 0x8F, 0x8F, 0xF8, 0x8F, 0x88, 0xF8, 0xFF, 0x88, 0xF8, 0x8F, 0xF8, 0x88, 0xF8, 0x8F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8F,
 0x88, 0x8F, 0xF7, 0x77, 0x71, 0x00, 0x00, 0x7F, 0x88, 0xFF, 0xF8, 0xFF, 0x88, 0xF8, 0x8F, 0x87, 0x8F, 0xF8, 0xFF, 0x8F, 0xFF, 0x8F, 0x88, 0x88, 0x78, 0x88, 0x8F, 0xF8, 0xFF, 0x8F, 0xFF, 0x8F,
 0x80, 0x8F, 0x8F, 0xFF, 0xFF, 0x88, 0x8F, 0x88, 0x87, 0x48, 0xFF, 0x8F, 0xF8, 0xF8, 0xF8, 0x78, 0xF8, 0x88, 0x88, 0xFF, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x88, 0x88, 0x88, 0x77,
 0x77, 0x55, 0x51, 0x18, 0xFF, 0x88, 0xFF, 0x8F, 0xFF, 0xF8, 0xF8, 0x64, 0x88, 0x88, 0x8F, 0xFF, 0x88, 0xFF, 0x8F, 0xF7, 0x7F, 0xF8, 0xF8, 0xFF, 0xF8, 0x8F, 0x88, 0xFF, 0x74, 0x7F, 0xFF, 0xFF,
 0xFF, 0xFF, 0x88, 0xFF, 0x86, 0x47, 0x8F, 0xFF, 0x88, 0x8F, 0x8F, 0x8F, 0xF8, 0xFF, 0x88, 0xFF, 0x74, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x88, 0x88, 0x8F, 0x87, 0x77, 0x77, 0x77, 0x71,
 0x8F, 0xF8, 0xF8, 0x88, 0xFF, 0xFF, 0x87, 0x44, 0xFF, 0xF8, 0x8F, 0x8F, 0x8F, 0xFF, 0xFF, 0x84, 0x7F, 0xFF, 0xF8, 0x88, 0xFF, 0x8F, 0xFF, 0xF7, 0x00, 0x08, 0xFF, 0x88, 0xF8, 0xFF, 0xFF, 0xF8,
 0x64, 0x74, 0x78, 0xFF, 0xFF, 0x8F, 0x8F, 0x78, 0xF8, 0x88, 0xFF, 0x88, 0x74, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x8F, 0x88, 0xFF, 0x88, 0x88, 0x88, 0x71, 0x07, 0xFF, 0xF8, 0xFF,
 0xFF, 0xF8, 0x74, 0x67, 0x88, 0x88, 0x88, 0x77, 0xFF, 0xFF, 0xF8, 0x74, 0x78, 0xF8, 0x88, 0x86, 0x8F, 0xFF, 0xF8, 0x74, 0x40, 0x04, 0x78, 0xFF, 0xF8, 0xF8, 0xFF, 0x80, 0x40, 0x06, 0x44, 0x78,
 0x8F, 0xF8, 0x88, 0x67, 0x8F, 0x88, 0xFF, 0x87, 0x74, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF8, 0x88, 0x88, 0xFF, 0xFF, 0xF8, 0xF8, 0x71, 0x07, 0x77, 0x88, 0xF8, 0x88, 0x70, 0x40, 0x00,
 0x07, 0x07, 0x77, 0x00, 0x77, 0x87, 0x70, 0x06, 0x00, 0x77, 0x00, 0x04, 0x07, 0x77, 0x70, 0x44, 0x04, 0x00, 0x07, 0x88, 0x8F, 0xF8, 0x87, 0x40, 0x00, 0x00, 0x66, 0x00, 0x67, 0x77, 0x60, 0x60,
 0x78, 0xFF, 0xFF, 0x87, 0x54, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8F, 0x88, 0x88, 0x88, 0x8F, 0xF8, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x8F, 0x80,
 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xFF, 0xFF, 0xF8, 0x88, 0xFF, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x70, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0x88, 0x88, 0x8F, 0xFF, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x78, 0x8F, 0xFF, 0x88, 0xFF, 0x87, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x8F,
 0xFF, 0xFF, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x77, 0x77, 0x70, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};


const unsigned char Red[] ={
 0x42, 0x4D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
 0x10, 0x00, 0x00, 0x00, // width 16
 0x10, 0x00, 0x00, 0x00, // height 16
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
 0x44, 0x44, 0x55, 0x11, 0x11, 0x55, 0x54, 0x44, // 16 by 16 image, 4 bits/pixel, upsided down
 0x44, 0x44, 0x55, 0x11, 0x11, 0x55, 0x77, 0x44,
 0x44, 0x45, 0x59, 0x99, 0x99, 0x71, 0x55, 0x54,
 0x44, 0x55, 0x99, 0x99, 0x99, 0x97, 0x51, 0x75,
 0x45, 0x59, 0x99, 0x99, 0x99, 0x99, 0x85, 0x55,
 0x55, 0x99, 0x99, 0x99, 0x99, 0x99, 0x97, 0x15,
 0x15, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x15,
 0x17, 0x19, 0x99, 0x99, 0x99, 0x99, 0x99, 0x11,
 0x17, 0x19, 0x99, 0x99, 0x99, 0x99, 0x99, 0x11,
 0x07, 0x11, 0x99, 0x99, 0x99, 0x99, 0x99, 0x11,
 0x05, 0x71, 0x99, 0x99, 0x99, 0x99, 0x99, 0x55,
 0x44, 0x71, 0x11, 0x99, 0x99, 0x99, 0x95, 0x55,
 0x44, 0x58, 0x11, 0x11, 0x99, 0x99, 0x55, 0x54,
 0x44, 0x45, 0x85, 0x51, 0x11, 0x95, 0x55, 0x44,
 0x44, 0x44, 0x07, 0x57, 0x75, 0x55, 0x44, 0x44,
 0x44, 0x44, 0x40, 0x11, 0x11, 0x14, 0x44, 0x44,
 0xFF,

};
const unsigned char Yellow[] ={
 0x42, 0x4D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
 0x10, 0x00, 0x00, 0x00, // width 16
 0x10, 0x00, 0x00, 0x00, // height 16
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
 0x44, 0x44, 0x00, 0x37, 0x77, 0x75, 0x54, 0x44, // 16 by 16 image, 4 bits/pixel, upsided down
 0x44, 0x44, 0x77, 0x88, 0x88, 0x77, 0x75, 0x44, 
 0x44, 0x07, 0x8B, 0x8B, 0x88, 0x88, 0x77, 0x74, 
 0x40, 0x78, 0xBB, 0xBB, 0xBB, 0xB8, 0x87, 0x75, 
 0x05, 0x87, 0xBB, 0xBB, 0xBB, 0xBB, 0x88, 0x77, 
 0x08, 0x83, 0xBB, 0xBB, 0xBB, 0xBB, 0xB8, 0x77, 
 0x18, 0x33, 0xBB, 0xBB, 0xBB, 0xBB, 0xB8, 0x87, 
 0x08, 0x3B, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0x87, 
 0x0F, 0x33, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0x83, 
 0x08, 0x33, 0xBB, 0xBB, 0xBB, 0xBB, 0xB8, 0x81, 
 0x07, 0x83, 0xBB, 0xBB, 0xBB, 0xBB, 0xB8, 0x85, 
 0x40, 0xF7, 0x3B, 0xBB, 0xBB, 0xBB, 0x87, 0x70, 
 0x44, 0x0F, 0x73, 0x33, 0xB3, 0x33, 0x87, 0x44, 
 0x44, 0x44, 0xF8, 0x73, 0x33, 0x88, 0x70, 0x44, 
 0x44, 0x44, 0x07, 0x88, 0x88, 0x70, 0x44, 0x44, 
 0x44, 0x44, 0x40, 0x00, 0x00, 0x00, 0x44, 0x44, 
0xFF,

};

const unsigned char Blank[] ={
 0x42, 0x4D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
 0x10, 0x00, 0x00, 0x00, // width 16
 0x10, 0x00, 0x00, 0x00, // height 16
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
 0x44, 0x44, 0x44, 0x44, 0x44, 0x77, 0x44, 0x44, // 16 by 16 image, 4 bits/pixel, upsided down
 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xF8, 0x74, 0x44,
 0x44, 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x54,
 0x44, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x74,
 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87,
 0x47, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7,
 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,
 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0x0F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0x47, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4,
 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4,
 0x44, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44,
 0x44, 0x47, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x44,
 0x44, 0x44, 0x6F, 0xFF, 0xFF, 0xF8, 0x74, 0x44,
 0x44, 0x44, 0x40, 0x46, 0x77, 0x44, 0x44, 0x44,
 0xFF,

};

const unsigned char Blue[] ={
 0x42, 0x4D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
 0x10, 0x00, 0x00, 0x00, // width 16
 0x10, 0x00, 0x00, 0x00, // height 16
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
 0x44, 0x44, 0x55, 0x11, 0x11, 0x55, 0x54, 0x44, // 16 by 16 image, 4 bits/pixel, upsided down
 0x44, 0x44, 0x55, 0x11, 0x11, 0x55, 0x77, 0x44,
 0x44, 0x45, 0x54, 0x44, 0x44, 0x71, 0x55, 0x54,
 0x44, 0x55, 0x44, 0x44, 0x44, 0x47, 0x51, 0x75,
 0x45, 0x54, 0x44, 0x44, 0x44, 0x44, 0x85, 0x55,
 0x55, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47, 0x15,
 0x15, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x15,
 0x17, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x11,
 0x17, 0x14, 0x44, 0x44, 0x44, 0x44, 0x44, 0x41,
 0x07, 0x14, 0x44, 0x44, 0x44, 0x44, 0x44, 0x11,
 0x05, 0x71, 0x44, 0x44, 0x44, 0x44, 0x44, 0x55,
 0x44, 0x71, 0x14, 0x44, 0x44, 0x44, 0x45, 0x55,
 0x44, 0x58, 0x11, 0x44, 0x44, 0x44, 0x55, 0x54,
 0x44, 0x45, 0x85, 0x54, 0x44, 0x45, 0x55, 0x44,
 0x44, 0x44, 0x07, 0x57, 0x75, 0x55, 0x44, 0x44,
 0x44, 0x44, 0x40, 0x11, 0x11, 0x14, 0x44, 0x44,
 0xFF,

};


const unsigned char Black[] ={
 0x42, 0x4D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
 0x10, 0x00, 0x00, 0x00, // width 16
 0x10, 0x00, 0x00, 0x00, // height 16
 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 16 by 16 image, 4 bits/pixel, upsided down
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0xFF,
};




int main(void){int i,j; 
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
  RIT128x96x4Init(1000000);
  // show the marquee
  RIT128x96x4_BMP(4, 86, Logo3);
  Delay(50000000);        // delay 3 sec at 50 MHz
  RIT128x96x4Clear();

  for(i=0; i<7; i++){  // top row is black
    RIT128x96x4_BMP(i*16, 16, Black);
  }
  for(i=0; i<7; i++){
		//Each iteration draws column i's 5 blank circles 
    for(j=2; j<7; j++){
      RIT128x96x4_BMP(i*16, j*16, Blank);
    }
  }
	// Makes a yellow circle go from right to left (stop at 2nd column)
	// Animates by drawing a black in its wake
  for(i=6;i>0;i--){
    RIT128x96x4_BMP(i*16, 16, Yellow);
     Delay(5000000);        // delay .3 sec at 50 MHz
     RIT128x96x4_BMP(i*16, 16, Black);
  }
	// Makes the yellow circle go from top to bottom of 2nd column
	// Animates by restoring a blank in its wake
  for(j=2;j<6;j++){
    RIT128x96x4_BMP(16, 16*j, Yellow);
    Delay(5000000);        // delay .3 sec at 50 MHz
    RIT128x96x4_BMP(16, 16*j, Blank);
  }
  RIT128x96x4_BMP(16, 16*6, Yellow); // Makes bottom row 2nd column Yellow
  Delay(50000000);        // delay 3 sec at 50 MHz
	// Makes a Blue circle go from right to left (till 3rd column)
	// Animates by drawing a black in its wake
  for(i=6;i>1;i--){ 
    RIT128x96x4_BMP(i*16, 16, Blue);
    Delay(5000000);        // delay .3 sec at 50 MHz
    RIT128x96x4_BMP(i*16, 16, Black);
  }
	// Makes a Blue circle go from top to bottom of 3rd column
	// Animates by restoring a blank in its wake
  for(j=2;j<6;j++){
    RIT128x96x4_BMP(32, 16*j, Blue);
    Delay(5000000);        // delay .3 sec at 50 MHz
    RIT128x96x4_BMP(32, 16*j, Blank);
  }
  RIT128x96x4_BMP(32, 16*6, Blue); // Makes bottom row 3rd column Blue
  Delay(500000000);        // delay 30 sec at 50 MHz 
  RIT128x96x4Clear();		// Clears screen
  while(1){

  }
}

