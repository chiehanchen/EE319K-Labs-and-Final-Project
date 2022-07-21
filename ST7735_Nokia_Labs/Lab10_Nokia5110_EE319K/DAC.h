// DAC.h
// Runs on LM4F120/TM4C123
// Use 9 resistors and PB3-0 to implement a simple 4-bit DAC.
// Daniel Valvano
// March 25, 2020

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex ",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020

 Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
#ifndef DAC_H
#define DAC_H
// resistor DAC bit 0 on PB0 (least significant bit)
// resistor DAC bit 1 on PB1
// resistor DAC bit 2 on PB2
// resistor DAC bit 3 on PB3 (most significant bit)

//********DAC_Init*****************
// Initialize PB3-0 as GPIO to implement the DAC.
// inputs: initial voltage output (0 to 15)
// outputs: none
void DAC_Init(uint8_t data);

//********DAC_Out*****************
// Output a value to the 4-bit DAC.
// inputs: voltage output (0 to 15)
// outputs: none
void DAC_Out(uint8_t data);

#endif

