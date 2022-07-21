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

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

value EQU	0 ; Binding
len	EQU	4	; Binding

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

    MACRO
    UMOD  $Mod,$Divnd,$Divsr ;MOD,DIVIDEND,DIVISOR
    UDIV  $Mod,$Divnd,$Divsr ;Mod = DIVIDEND/DIVISOR
    MUL   $Mod,$Mod,$Divsr   ;Mod = DIVISOR*(DIVIDEND/DIVISOR)
    SUB   $Mod,$Divnd,$Mod   ;Mod = DIVIDEND-DIVISOR*(DIVIDEND/DIVISOR)
    MEND
  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {LR, R0}

; First, we allocate spaces
	SUB	SP,	#8;
	
; Now we access our values
	CMP R0, #10
	BLO stillMore
	MOV R3, #0xA
	UDIV R2, R0, R3
	MLS R1, R2, R3, R0 
	MOV R0, R2
	STR R1, [SP, #value]
	BL LCD_OutDec
	LDR R0, [SP, #value]
	ADD R0, #0x30
	BL ST7735_OutChar
	B allSet
stillMore
	ADD R0, #0x30
	BL ST7735_OutChar
allSet

	; Deallocate Stack
	
	ADD	SP,	#8

	POP {LR, R0}

      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	MOV R1, #0
moreToDo
	PUSH {LR, R4, R1, R0}
	
; ******Allocation Phase*******	
	SUB SP, #8 ; Allocate num

; *****Access Phase*******
	MOV R4, #10000
	CMP R0, R4	;If number is too big then we need to output the stars
	BHS pound
	CMP R1, #4	; Check if we have reached the last character
	BEQ done
	MOV R3, #0xA	
	UDIV R2, R0, R3	; Shift the character to the first position
	MLS R4, R2, R3, R0 
	MOV R0, R2
	STR R4, [SP, #value]
	STR R1, [SP, #len]
	ADD R1, #1
	BL moreToDo
	LDR R0, [SP, #value]
	ADD R0, #0x30	; add 0x30 after the decimal number we want to output because ASCII char is 0x3#
	BL ST7735_OutChar
	LDR R1, [SP, #len]
	CMP R1, #3
	BNE done
	
	MOV R0, #0x2E	; Output the period
	BL ST7735_OutChar
	B done

pound
	MOV R0, #0x2A ; output the first * before the period
	BL ST7735_OutChar
	MOV R0, #0x2E ; output the period "."
	BL ST7735_OutChar
	MOV R0, #0x2A ; output the next * after the period
	BL ST7735_OutChar
	MOV R0, #0x2A ; output the second * before the period
	BL ST7735_OutChar
	MOV R0, #0x2A ; output the last * before the period
	BL ST7735_OutChar
done

	
;****Deallocation Phase*****	
	ADD SP, #8
	
	
	POP {LR, R4, R1, R0}

     BX   LR
;* * * * * * * * End of LCD_OutFix * * * * * * * *
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file