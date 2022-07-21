// SpaceInvaders.c  
// Runs on TM4C123
// Jonathan Valvano
// March 25, 2020
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2020

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
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
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
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total
// ----------------------------------
#include <stdint.h>

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#include "../inc/tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "sound.h"
#include "pll.h"
#include "timer0.h"
#include "timer1.h"
#include "ADC.h"
#include "dac.h"
#include "print.h"
#include "random.h"
#include "image.h"

// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

int main(void){int PlayerPosition; uint32_t time=9;
  PLL_Init(Bus80MHz);   // set system clock to 80 MHz
  DisableInterrupts();          // start critical section
  Nokia5110_Init();
  ADC_Init();
  Sound_Init();
  // foreground updates the screen
  Nokia5110_Clear();
  Nokia5110_ClearBuffer();              // remove the marquee from the buffer
  EnableInterrupts();           // end of critical section
  
  while(time>0){
    for(PlayerPosition=10; PlayerPosition<(SCREENW-25); PlayerPosition+=2){
      Nokia5110_PrintBMP(PlayerPosition, SCREENH-1, PlayerShip0, 0);
      Nokia5110_DisplayBuffer(); 
      Nokia5110_SetCursor(7,0);Nokia5110_OutUDec(time);
      Delay100ms(1);         
    }
    for(PlayerPosition=(SCREENW-25); PlayerPosition>10; PlayerPosition-=2){
      Nokia5110_PrintBMP(PlayerPosition, SCREENH-1, PlayerShip0, 0);
      Nokia5110_DisplayBuffer();
      Nokia5110_SetCursor(7,0);Nokia5110_OutUDec(time);
      Delay100ms(1);         
    }
    time--;
  }
  Nokia5110_Clear();
  Nokia5110_SetCursor(0,0);
  Nokia5110_OutString("Game over");
  while(1){
  }
}


