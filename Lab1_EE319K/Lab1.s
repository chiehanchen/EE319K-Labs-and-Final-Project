;****************** Lab1.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Chieh-An Chen
; Date Created: 1/15/2018 
; Last Modified: 1/10/2022
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1
GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       PRESERVE8 
       AREA   Data, ALIGN=4
; No global variables needed

       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "cc67994",0  ;replace abc123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 1 ; change to nonzero when ready for grading
           
      EXPORT  Lab1
Lab1 
;Initializations
PortDInit
	LDR R0, = SYSCTL_RCGCGPIO_R	;load address
	LDR R1, [R0]				;load contents
	ORR R1, R1, #8				;set bit 3 to 1 
	STR R1, [R0]				;store back to R0
	NOP							;  stabilize clock signal
	NOP
	
	LDR R0, =GPIO_PORTD_DIR_R	;direction
	LDR R1, [R0]
	ORR R1, R1, #32				;sets PD5 to output
	BIC R1, R1, #7				;sets PD0,1,2 to input
	STR R1, [R0]
	
	LDR R0, = GPIO_PORTD_DEN_R	;enable
	LDR R1, [R0]
	ORR R1, R1, #39				;enable pins 0,1,2,3,5
	STR R1, [R0]	 


loop
;input, calculate, output  
	MOV R1, #0x03750042
	MOV	R2, #0x50820083
	AND	R0, R1, R2
	LDR R0, =GPIO_PORTD_DATA_R
	LDR R1, [R0]; for PD0
	BIC	R1, R1, #6
	
	LDR R2, [R0]; for PD1
	BIC	R2, R2, #5
	LSR	R2, #1
	
	LDR R3, [R0]; for PD2
	BIC	R3, R3, #3
	LSR	R3, #2
	
	ADD	R4, R1, R2
	ADD	R4, R4, R3
	BIC	R4, R4, #2
	LSL R4, #5

	
	STR	R4, [R0]
    B    loop


    
    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file

               