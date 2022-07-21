// Lab5.c starter program EE319K Lab 5, Spring 2022
// Runs on TM4C123
// Chieh-An Chen and Francis King
// Last Modified: 1/11/2021

/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "cc67994"; //  ;replace abc123 with your EID
char EID2[] = "fmk325"; //  ;replace abc123 with your EID

struct state{
	uint8_t outTrafficLED;	// Represents LED output; this output is the port for the traffic light LEDs (PORT E)
	uint8_t outWalkLED;		  // Represents the bit number of the output; this output is the port for the walk LED (represents PORT F)
	uint8_t wait;				    // Represents the delay time between each signal or each SysTick
	uint8_t next[8]; 			  // Number of NEXT States cooresponding to each input 
};

//PA4 Walk
//PA3 South
//PA2 West


// input values for port A: 
// 0x0=000= no sensors
// 0x1=001= west sensor
// 0x2=010= south sensor
// 0x3=011= west & south sensors
// 0x4=100= walk sensor
// 0x5=101= walk & west sensors
// 0x6=110= walk & south sensors
// 0x7=111= walk & west & south sensors

// output values for port B:
// 0x21=00100001; - PB5&PB0 --> Red West / Green South
// 0x22=00100010; - PB2&PB4 --> Red West / Yellow South
// 0x0C=00001100; - PB0&PB5 --> Green West / Red South
// 0x14=00010100; - PB1&PB5 --> Yellow West / Red South
// 0x24=00100100; - PB2&PB5 --> Red West / Red South

// output values for port F:
// 0x2=0010; --> PF1 is ON -> don't walk
// 0x8=1000; --> PF3 is ON -> do walk
// 0xE=1110; --> PF3, PF2, PF1 is ON --> WHITE LED

#define LIGHT                   (*((volatile uint32_t *)0x400050FC))
#define GPIO_PORTB_OUT          (*((volatile uint32_t *)0x400050FC)) // bits 5-0
#define GPIO_PORTE_IN           (*((volatile uint32_t *)0x4002400C)) // bits 1-0
#define SENSOR                  (*((volatile uint32_t *)0x4002400C))

#define start 0		// Start state --> Walk LED, South & West are all RED
#define gS		1 	// Green South
#define yS		2		// Yellow South
#define rS		3 	// Red South
#define gW		4		// Green West
#define yW		5		// Yellow West
#define rW		6		// Red West
#define walk	7		// Walk LED --> PF1, PF2, PF3 are all on, --> 00001110
#define on1		8		// First Warning LED ON
#define off1	9		// First Warning LED OFF
#define on2		10	// Second Warning LED ON
#define off2	11	// Second Warning LED OFF
#define on3		12	// Third Warning LED ON
#define off3	13	// Third Warning LED OFF
#define dWalk	14	// Don't Walk LED --> Red LED kept ON --> PF1 is ON
uint8_t input;
uint8_t current = start;
uint32_t portA;
//                                  PSW  PSW	PSW	 PSW  PSW	 PSW  PSW	 PSW  
// Next state notation as follows: {000, 001, 010, 011, 100, 101, 110, 111}
struct state FSM[15] = { 			/* Number of States in the FSM*/
	{0x24, 0x2, 100, {start, gW, gS, gS, walk, walk, gS, gS}},	// "Start" state, walkLED, southLED, & westLED all RED
	
	{0x21, 0x2, 100, {yS, yS, gS, yS, yS, yS, yS, yS}},															//gS - Green South
	{0x22, 0x2, 50, {rS, rS, rS, rS, rS, rS, rS, rS}},															//yS - Yellow South
	{0x24, 0x2, 100, {start, gW, start, gW, walk, gW, walk, walk}},									//rS - Red South
	
	{0x0C, 0x2, 100, {yW, gW, yW, yW, yW, yW, yW, yW}},															//gW - Green West
	{0x14, 0x2, 50, {rW, rW, rW, rW, rW, rW, rW, rW}},															//yW - Yellow West
	{0x24, 0x2, 100, {start, start, start, start, start, start, start, start}},			//rW - Red West
	
	{0x24, 0xE, 100, {on1, on1, on1, on1, walk, on1, on1, on1}},											//walk - Walk
	{0x24, 0x2, 50, {off1, off1, off1, off1, on1, off1, off1, off1}},							//on1
	{0x24, 0x0, 50, {on2, on2, on2, on2, off1, on2, on2, on2}},										//off1
	{0x24, 0x2, 50, {off2, off2, off2, off2, on2, off2, off2, off2}},							//on2
	{0x24, 0x0, 50, {on3, on3, on3, on3, off2, on3, on3, on3}},										//off2
	{0x24, 0x2, 50, {off3, off3, off3, off3, on3, off3, off3, off3}},							//on3
	{0x24, 0x0, 50, {dWalk, dWalk, dWalk, dWalk, off3, dWalk, dWalk, dWalk}},			//off3
	{0x24, 0x2, 100, {start, gW, gS, gS, walk, gW, gS, gW}},			//dWalk	- Don't Walk
};

void DisableInterrupts(void);
void EnableInterrupts(void);

int main(void){ volatile uint32_t delay;
  DisableInterrupts();
  TExaS_Init(GRADER);
	
	SYSCTL_RCGC2_R |= 0x31; // Ports B,F,A
  delay = SYSCTL_RCGC2_R;
	
  SysTick_Init();   // Initialize SysTick for software waits
	
	SYSCTL_RCGCGPIO_R |= 0x31; // real clock register
	
	SYSCTL_RCGC2_R |= 0x31;  // LM3S legacy clock register
  delay = SYSCTL_RCGC2_R;
	
	GPIO_PORTE_DIR_R |= 0x3F;
	GPIO_PORTE_DEN_R |= 0x3F;
	GPIO_PORTA_DIR_R &= 0x00;
	GPIO_PORTA_DEN_R |= 0x38;
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_DEN_R |= 0x0E;
  // initialize system
  EnableInterrupts();
	
	 // Input is what drives the transition
		// Now, currentState is OFF
  while(1){
		GPIO_PORTE_DATA_R = FSM[current].outTrafficLED;  // Output for Traffic LED Lights (north/south & east/west)
		GPIO_PORTF_DATA_R = FSM[current].outWalkLED;		 // Output	for Walk LED Lights
		SysTick_Wait10ms(FSM[current].wait);						 // Wait/Delay Timing with SysTick
		portA = GPIO_PORTA_DATA_R;
		input = (portA & 0x38) >> 3;											 // Input
		current = FSM[current].next[input];
		
    // 1) output
    // 2) wait
    // 3) input
    // 4) next

  }
}





