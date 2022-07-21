;****************** Lab3.s ***************
; Program written by: Chieh-An Chen and Francis King
; Date Created: 2/4/2017
; Last Modified: 1/10/2022
; Brief description of the program
;   The LED toggles at 2 Hz and a varying duty-cycle
; Hardware connections (External: Two buttons and one LED)
;  Change is Button input  (1 means pressed, 0 means not pressed)
;  Breathe is Button input  (1 means pressed, 0 means not pressed)
;  LED is an output (1 activates external LED)
; Overall functionality of this system is to operate like this
;   1) Make LED an output and make Change and Breathe inputs.
;   2) The system starts with the the LED toggling at 2Hz,
;      which is 2 times per second with a duty-cycle of 30%.
;      Therefore, the LED is ON for 150ms and off for 350 ms.
;   3) When the Change button is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 30% to 70% to 70%
;      to 90% to 10% to 30% so on
;   4) Implement a "breathing LED" when Breathe Switch is pressed:
; PortE device registers
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608

        IMPORT  TExaS_Init
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID1
EID1   DCB "cc67994",0  ;replace ABC123 with your EID
       EXPORT EID2
EID2   DCB "fmk325",0  ;replace ABC123 with your EID
       ALIGN 4

     EXPORT  Start

Start
; TExaS_Init sets bus clock at 80 MHz, interrupts, ADC1, TIMER3, TIMER5, and UART0
     MOV R0,#2  ;0 for TExaS oscilloscope, 1 for PORTE logic analyzer, 2 for Lab3 grader, 3 for none
     BL  TExaS_Init ;enables interrupts, prints the pin selections based on EID1 EID2
 ; Your Initialization goes here
	LDR	 R0, =SYSCTL_RCGCGPIO_R
	LDRB R1, [R0]
	ORR	 R1, #16
	STRB R1, [R0]
	NOP
	NOP
	
	LDR R0, =GPIO_PORTE_DIR_R	;direction
	LDR R1, [R0]
	ORR R1, R1, #32				;sets PE5 to output
	BIC R1, R1, #10				;sets PE1,3 to input
	STR R1, [R0]
	
	LDR R0, = GPIO_PORTE_DEN_R	;enable
	LDR R1, [R0]
	ORR R1, R1, #42 			;enable pins 1,3,5
	STR R1, [R0]	 
	
	CPSIE	I
	MOV R3, #50 ;seconds counter
	MOV R2, #3  ;percent
	MOV	R4, #0
count EQU 20000


loop  
; main engine goes here
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R6, [R0] 					;R6 HOLDS PAST REG VALUE
	AND R6, #0X02
	MOV R5, #750					;600
	MOV R10, #1650					;1400	

	
change	
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R7, [R0]
	AND R7, #0X08
	CMP R7, #0
	BNE BREATHE

	LDR R0, =GPIO_PORTE_DATA_R
	LDR R7, [R0]
	AND R7, #0X02
	ADD R9, R7, R6						; COMPARE PREVIOUS AND CURRENT 
	CMP R9, #4
	BEQ change							; BRANCH IF 1>1
	CMP R7, R6
	BLO increment						;BRANCH IF 1>0
	ADD R6, R7, #0
	BHI change							; BRANCH IF 0>1					
	BEQ sameduty


sameduty	
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R8, [R0] ;Load R8 with Port E Data
	ORR	R8, R8, #0x20 ;OR with 0010 0000 5th bit/turn on LED
	STR	R8, [R0] ;Store new DATA in address in R0
	MUL	R5, R2, R3 		;Multiply 50 seconds with 3 and store to R5 to get initial duty cycle
	BL	delay	;Unconditional branch to delay
	
LEDOFF	
	LDR	R8, [R0] 
	AND	R8, #0xDF
	STR	R8, [R0]
	MOV	R5, #10
	SUBS R5, R5, R2
	MUL	R5, R5, R3
	BL delay
		
	B    loop
 
increment
	ADD	R2, R2, #2
	AND	R7, R7, #0
	AND	R6, R6, #0
	CMP	R2, #11
	BEQ	backto10
back
	BL	sameduty
	
delay 
	PUSH {R1, R5} ;Save states of R1 and R5
    MOV R1, #18500 ;Load R1 with 8889
    MUL R5, R5, R1 ;Multiply 8889 with value of R5
change_pressed			; check if the change button is pressed
    SUBS R5, R5, #1 ;subtract R5 by 1
    BNE  change_pressed ;if value in R5 does not equal 0, branch back to change_pressed
    POP {R1, R5};pop R1 and R5
	ADD	R9, R9, #1
	AND	R9, R9, #1
	CMP	R9, #1
	BNE change;return to call function
	BL	LEDOFF
	
backto10
	ADD	R2, R2, #-10
	BL back
  
  
  
  
  
  

BREATHE					;Breath function calls breath2 which is the basic duty cycle
	PUSH{R2, R3, R6, R9} ;Save States of R2, R3, R6 and R9
	MOV R7, #3
	MOV R9, #0
	
CYCLE20
	ADD R0, R0, #0
LEDONB
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R8, [R0] ;Load R8 with Port E Data
	ORR	R8, R8, #0x20 ;OR with 0010 0000 5th bit/turn on LED
	STR	R8, [R0] ;Store new DATA in address in R0
	MUL	R5, R2, R3 		;Multiply 50 seconds with 3 and store to R5 to get initial duty cycle

	BL	delaybon	;Unconditional branch to delay
	
		
		
LEDOFFB
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R8, [R0] ;Load R8 with Port E Data
	BIC	R8, R8, #0x20 ;OR with 0010 0000 5th bit/turn on LED
	STR	R8, [R0] ;Store new DATA in address in R0
	MOV R5, #10
	SUB R5, R5, R2
	MUL R5, R5, R3
	BL	delaybon	;Unconditional branch to delay
	BL	counter
backb
	ADD R2, R2, #1
	CMP R2, #10
	BNE INCREMENT
	BL DECREMENT
		
INCREMENT
	ADD R2, R2, #-1
	ADD R2, R2, #1
	MOV R7, #3
	BL change
		
DECREMENT
	SUBS R2, R2, #1
LEDONBD
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R8, [R0] ;Load R8 with Port E Data
	ORR	R8, R8, #0x20 ;OR with 0010 0000 5th bit/turn on LED
	STR	R8, [R0] ;Store new DATA in address in R0
	MUL	R5, R2, R3 		;Multiply 50 seconds with 3 and store to R5 to get initial duty cycle

	BL	delaybon	;Unconditional branch to delay
	
		
LEDOFFBD
	LDR R0, =GPIO_PORTE_DATA_R
	LDR R8, [R0] ;Load R8 with Port E Data
	BIC	R8, R8, #0x20 ;OR with 0010 0000 5th bit/turn on LED
	STR	R8, [R0] ;Store new DATA in address in R0
	MOV R5, #10
	SUB R5, R5, R2
	MUL R5, R5, R3
	BL	delaybon	;Unconditional branch to delay
	BL	counterd
backbd
	CMP R2, #0
	BEQ change
	BL	DECREMENT

delaybon
	PUSH{R1, R5}
	MOV R1, #5000
	MUL R5, R1, R5
loop1bon
	SUBS R5, R5, #1
	BNE loop1bon
	POP {R1, R5}
	BX LR
	
counter
	SUBS R7, R7, #1
	BNE	LEDONB
	MOV R7, #3
	BL backb
	
counterd
	SUBS R7, R7, #1
	BNE	LEDONBD
	MOV R7, #3
	BL backbd
	
	




     ALIGN      ; make sure the end of this section is aligned
     END        ; end of file





