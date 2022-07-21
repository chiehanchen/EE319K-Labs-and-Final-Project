; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
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

    AREA    |.text|, CODE, READONLY, ALIGN=2
    PRESERVE8
    THUMB
count EQU  0
num EQU 4  
  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"
LCD_OutDec
   PUSH {R4-R10,LR}
   MOV R9, #1
   SUB SP, #8
   MOV R11, SP
   MOV R2, #0
   STR R2, [R11, #count]
   STR R0, [R11, #num]
   MOV R3, #10
   LDR R2, [R11, #num]; first time
   UDIV R4, R2, R3
   MUL R6, R4, R3
   SUB R7, R2, R6
   PUSH {R7}
   STR R4, [R11, #num]
   LDR R4, [R11, #count]
   ADD R4, #1
   STR R4, [R11, #count]
looper LDR R2, [R11, #num]
   cmp R2, #0
   BEQ next
   UDIV R4, R2, R3
   MUL R6, R4, R3
   SUB R7, R2, R6
   PUSH {R7}
   STR R4, [R11, #num]
   LDR R4, [R11, #count]
   ADD R4, #1
   STR R4, [R11, #count]
   B looper
next  LDR R4, [R11, #count]
   cmp R4, #0
   BEQ done
   POP {R0}
   ADD R0, #0x30
   BL ST7735_OutChar
   LDR R4, [R11, #count]
   SUB R4, #1
   STR R4, [R11, #count]
   B next
done 
   ADD SP, #8
   POP {R4-R10,LR}
      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
   PUSH {R4-R10,LR}
   SUB SP, #8
   MOV R9, #1
   LDR R10, =9999
   cmp R0, R10
   BHI badtime
   MOV R11, SP
   MOV R2, #0
   STR R2, [R11, #count]
   STR R0, [R11, #num]
   MOV R3, #10
   LDR R2, [R11, #num]; first time
   UDIV R4, R2, R3
   MUL R6, R4, R3
   SUB R7, R2, R6
   PUSH {R7}
   STR R4, [R11, #num]
   LDR R4, [R11, #count]
   ADD R4, #1
   STR R4, [R11, #count]
looper2 LDR R2, [R11, #num]
   cmp R2, #0
   BEQ next2
   UDIV R4, R2, R3
   MUL R6, R4, R3
   SUB R7, R2, R6
   PUSH {R7}
   STR R4, [R11, #num]
   LDR R4, [R11, #count]
   ADD R4, #1
   STR R4, [R11, #count]
   B looper2
next2  LDR R4, [R11, #count]
   cmp R4, #0
   BEQ done2
   cmp R4, #4
   BLO addzeroes
return   POP {R0}
   ADD R0, #0x30
   BL ST7735_OutChar
   LDR R4, [R11, #count]
   SUB R4, #1
   STR R4, [R11, #count]
   cmp R9, #1
   BNE next3
   MOV R0, #46
   BL ST7735_OutChar
   SUB R9, #1
next3   LDR R4, [R11, #count]
   cmp R4, #0
   BEQ done2
   POP {R0}
   ADD R0, #0x30
   BL ST7735_OutChar
   LDR R4, [R11, #count]
   SUB R4, #1
   STR R4, [R11, #count]
   cmp R9, #1
   BNE next3
   MOV R0, #46
   BL ST7735_OutChar
   SUB R9, #1
   B next3

addzeroes  MOV R0, #0x30
   BL ST7735_OutChar
   MOV R0, #46
   BL ST7735_OutChar
   LDR R5, =4
   SUB R5, R4
   SUB R9, #1
morezeroes   cmp R5, #1
   BEQ return
   MOV R0, #0x30
   BL ST7735_OutChar
   SUB R5, #1
   B morezeroes

badtime
   MOV R0, #42
   BL ST7735_OutChar
   MOV R0, #46
   BL ST7735_OutChar
   MOV R0, #42
   BL ST7735_OutChar
   MOV R0, #42
   BL ST7735_OutChar
   MOV R0, #42
   BL ST7735_OutChar
   
   
   
done2 
   ADD SP, #8
   POP {R4-R10,LR}
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file


