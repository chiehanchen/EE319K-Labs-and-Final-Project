// Lab7Main.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 1/17/2020
// Ramesh Yerraballi modified 3/20/2017

// Font table, initialization, and other functions based
// off of Nokia_5110_Example from Spark Fun:
// 7-17-2011
// Spark Fun Electronics 2011
// Nathan Seidle
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/LCD/Monochrome/Nokia_5110_Example.pde

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
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

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected

// Blue Adafruit 338 Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Ground        (Gnd, pin 1) ground
// 3.3V          (Vcc, pin 2) power
// SSI0Clk       (Clk, pin 3) connected to PA2
// SSI0Tx        (Din, pin 4) connected to PA5
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Fss       (CS,  pin 6) connected to PA3
// Reset         (RST, pin 7) connected to PA7
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include <stdint.h>
#include "Nokia5110.h"
#include "PLL.h"
#include "IO.h"
#include "Print.h"


// image of a longhorn loves 319k
const uint8_t Longhorn2[] = {
  0x08, 0x08, 0x08, 0x08, 0x08, 0x18, 0x18, 0x18, 0x38, 0x30, 0x30, 0x30, 0x70, 0xF0,
  0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xF0, 0x70,
  0x70, 0x30, 0x30, 0x18, 0x18, 0x18, 0x18, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
  0xF0, 0x08, 0x04, 0x24, 0x24, 0xE4, 0x24, 0x24, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08,
  0xF1, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x0E, 0x0C, 0x1C, 0x38, 0x38, 0xB8, 0xF8, 0xF0,
  0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
  0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0,
  0xF0, 0x78, 0x38, 0x3C, 0x1C, 0x1F, 0x0F, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0x00, 0x00, 0x04, 0x04, 0x07, 0x84, 0x44, 0x40, 0x80, 0x40, 0x40, 0x80, 0x00,
  0x00, 0x01, 0x02, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0F, 0x0F, 0x0F, 0x0F,
  0x0F, 0x07, 0x07, 0x07, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x1F, 0x1E, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x0B, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x84, 0x88, 0x84, 0x02, 0x01, 0x00,
  0x00, 0x80, 0x00, 0x00, 0x01, 0x82, 0x84, 0x84, 0x84, 0x04, 0x04, 0x84, 0x04, 0x04,
  0x84, 0x08, 0x10, 0xE0, 0x00, 0x01, 0x07, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0F, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x88, 0x88, 0x8A, 0x85, 0x80, 0x80,
  0x89, 0x8F, 0x88, 0x80, 0x80, 0x83, 0x82, 0x82, 0x8F, 0x80, 0x80, 0x8F, 0x82, 0x85,
  0x88, 0x80, 0x80, 0x8F, 0x50, 0x20, 0x00, 0x3C, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x7F, 0x3F, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define SIZE 16
uint32_t const TestData[SIZE] ={
  0,7,9,10,99,100,409,654,999,1000,9999,10000,20806,65535,
  123400009,0xFFFFFFFF
};

  
int main(void){uint32_t i;
  PLL_Init(Bus80MHz);         // set system clock to 80 MHz
  IO_Init();
  Nokia5110_Init();
  Nokia5110_Clear();
  Nokia5110_OutString("Lab 7\nSpring 2020\nWelcome to \nEE319K");
  IO_Touch();
  Nokia5110_Clear();
  Nokia5110_DrawFullImage(Longhorn2);
  IO_Touch();
  Nokia5110_Clear();
  for(i=0;i<SIZE;i++){
    IO_HeartBeat();
    LCD_OutDec(TestData[i]);
    Nokia5110_OutString("\n   ");
    LCD_OutFix(TestData[i]);
    IO_Touch(); 
    Nokia5110_OutString("\n\n");
  }
  while(1){
  }
}
