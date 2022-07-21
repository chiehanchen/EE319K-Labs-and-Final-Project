; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 
	PRESERVE8

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

;binding
ST EQU 0
ST2 EQU 4

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
; lab 7 code

	PUSH{R4-R11, LR} ;PUSH LINK REGISTER
	SUB SP, #8
	CMP R0, #10 ;COMPARE IF RO < 10
	BLO DONE ;IF SO SKIP
	MOV R2, #10 ;R2 = 10
	UDIV R3, R0, R2 ;R0/10 = R3
	MUL R1, R3, R2 ;R3 * 10 FOR PLACE VALUE
	SUB R1, R0, R1 ;R0
	MOV R11, #ST
	;PUSH{R1} ;PUSH R1 BEACUSE THE WAY THE PRINT FUNNCTION WORKS, IT HAS TO READ FROM THE STACK
	STRB R1, [SP, R11] ;STORE R1 ONTO THE STACK
	MOVS R0, R3 ;CLONE R3 BECAUSE YOU KEEP GOING WITH REMAINDER
	BL LCD_OutDec ;LOOP FOR THE NEXT CHARACTER
	LDRB R0, [SP, R11]
	;POP{R0} ;POP R0, BECAUSE
DONE
	ADD R0, #'0' ;ADD THE ASCII VALUE TO R0
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	ADD SP, #8
	POP{R4-R11, LR} ;PUT BACK THE PRINT FUNCTION

      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
; lab 7 code

	PUSH{R4-R11, LR} ;PUSH LINK REGISTER
	SUB SP, #8
	MOV R11, #ST2
	MOV R1, #9999
	CMP R0, R1 ;COMPARE IF RO < 10
	BHI LARGE ;IF SO SKIP


	MOV R1, #1000
	MOV R2, R0
	UDIV R0, R1
	MUL R1, R0
	SUB R2, R1
	ADD R0, #0x30

	PUSH{LR, R2}
	BL ST7735_OutChar
	MOV R0, #"."
	BL ST7735_OutChar
	POP {LR, R2}
	MOV R1, #100
	UDIV R0,R2, R1
	MUL R1, R0
	SUB R2, R1
	ADD R0, #0x30

	PUSH{LR, R2}
	BL ST7735_OutChar
	POP {LR, R2}
	MOV R1, #10
	UDIV R0,R2, R1
	MUL R1, R0
	SUB R2, R1
	ADD R0, #0x30

	PUSH{LR, R2}
	BL ST7735_OutChar
	POP {LR, R2}
	MOV R0, R2
	ADD R0, #0x30

	PUSH{LR, R2}
	BL ST7735_OutChar
	POP {LR, R2}
	BX LR

LARGE
	MOV R0, #'*'
	STRB R0, [SP, R11] ;STORE R1 ONTO THE STACK
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	LDRB R0, [SP, R11]
;ADD R11, #1

	MOV R0, #'.'
	STRB R0, [SP, R11] ;STORE R1 ONTO THE STACK
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	LDRB R0, [SP, R11]
;ADD R11, #1

	MOV R0, #'*'
	STRB R0, [SP, R11] ;STORE R1 ONTO THE STACK
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	LDRB R0, [SP, R11]
;ADD R11, #1

	MOV R0, #'*'
	STRB R0, [SP, R11] ;STORE R1 ONTO THE STACK
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	LDRB R0, [SP, R11]
;ADD R11, #1

	MOV R0, #'*'
	STRB R0, [SP, R11] ;STORE R1 ONTO THE STACK
	BL ST7735_OutChar ;RUN THE PRINT COMMAND
	LDRB R0, [SP, R11]
;ADD R11, #1

	ADD SP, #8
	POP{R4-R11, LR} ;PUT BACK THE PRINT FUNCTION

     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
