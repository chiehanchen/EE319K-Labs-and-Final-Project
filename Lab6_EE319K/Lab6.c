// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/11/22  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "cc67994"; //  ;replace abc123 with your EID
char EID2[] = "fmk325"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;

// lab video Lab6_voltmeter, Program 6.1
// A simple program that outputs sixteen DAC values. Use this main if you have a voltmeter.
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SIMULATIONGRADER);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}

// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static. Program 6.2
// A simple program that outputs sixteen DAC values. 
// Use this main if you do not have a voltmeter. 
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}


     
int main(void){       
  DisableInterrupts();
  TExaS_Init(REALBOARDGRADER);    // bus clock at 80 MHz
  Key_Init(); 
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
  EnableInterrupts();
  while(1){                
    uint32_t button;
		button = Key_In();
		
		if(button== 0){
			Sound_Start(0);
		}
		//1/ [(12.5ns*64)(frequency)(1,000,000,000)]
		
		//80M/(64*frequency)
		if(button == 1){ // Port A being high means that data R = 0100 = 4
			Sound_Start(4018); // frequency = 311.1Hz
		}
		if(button == 2){
			Sound_Start(3190); // frequency = 392.0 Hz
		}
		if(button == 4){
			Sound_Start(2681); // frequency = 466.2 Hz
		}
		if(button == 8){
			Sound_Start(2390); // frequency = 523.3 Hz
		}
		
  }                        
}


