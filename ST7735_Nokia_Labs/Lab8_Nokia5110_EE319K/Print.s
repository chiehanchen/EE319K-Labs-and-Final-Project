; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; Nokia5110_OutCharoutputs a single 8-bit ASCII character
; Nokia5110_OutStringoutputs a null-terminated string 

    IMPORT   Nokia5110_OutChar
    IMPORT   Nokia5110_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

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
;  1) allocate local variable n on the stack
    PUSH {R0,R4,R5,LR}
    SUB  SP, #8   ;allocate n
n   EQU  0
;  2) set n with the input parameter passed in R0
    STR R0, [SP,#n]
;  3) if(n >= 10){
    CMP  R0, #10
    BLO  outDecDone
    MOV  R4, #10
    UDIV R0,R0,R4   ;n/10
    BL   LCD_OutDec ;LCD_OutDec(n/10);
    LDR  R1, [SP,#n]
    UMOD R0,R1,R4   ; n = n%10;
outDecDone
;  4) ST7735_OutChar(n+$30); /* n is between 0 and 9 */
    ADD  R0,#0x30  ; ASCII 0 to 9
    BL   Nokia5110_OutChar
;  5) deallocate variable
    ADD  SP,#8
    POP  {R0,R4,R5,PC}

      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.01, range 0.00 to 9.99
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.00 "
;       R0=3,    then output "0.03 "
;       R0=89,   then output "0.89 "
;       R0=123,  then output "1.23 "
;       R0=999,  then output "9.99 "
;       R0>999,  then output "*.** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
;  0)save any registers that will be destroyed by pushing on the stack
;  1)allocate local variables letter and num on the stack
         PUSH {R4,LR}
         SUB  SP,#8
letter   EQU  0   ;8-bit character (32-bit variable)
num      EQU  4   ;16-bit
;  2)initialize num to input parameter, which is the integer part
         STR  R0,[SP,#num]
;  3)if number is less or equal to 999, go the step 6
         LDR R1,=999
         CMP R0,R1
         BLS  outfix6
;  4)output the string "*.** " calling ST7735_OutString
         LDR R0,=errOutFix
         BL  Nokia5110_OutString
;  5)go to step 19
         B    outfix19
;  6)perform the division num/100, putting the quotient in letter,
;      and the remainder in num
outfix6
         LDR  R0,[SP,#num]
         MOV  R1,#100
         UDIV R2,R0,R1  ;R2=num/100
         UMOD R3,R0,R1  ;R3=num%100
         STR  R3,[SP,#num]
         STR  R2,[SP,#letter]
;  7)convert the ones digit to ASCII, letter = letter+$30
         LDR  R0,[SP,#letter]
         ADD  R0,#0x30
;  8)output letter to the LCD by calling ST7735_OutChar
         BL  Nokia5110_OutChar
;  9)output "." to the LCD by calling ST7735_OutChar
         MOV R0,#'.'
         BL  Nokia5110_OutChar
;  10)perform the division num/10, putting the quotient in letter,
;      and the remainder in num
         LDR  R0,[SP,#num]
         MOV  R1,#10
         UDIV R2,R0,R1  ;R2=num/10
         UMOD R3,R0,R1  ;R3=num%10
         STR  R3,[SP,#num]
         STR  R2,[SP,#letter]
;  11)convert the tenths digit to ASCII, letter = letter+$30
         LDR  R0,[SP,#letter]
         ADD  R0,#0x30
;  12)output letter to the LCD by calling ST7735_OutChar
         BL  Nokia5110_OutChar
;  13)get num
         LDR  R0,[SP,#num]
;  14)convert the hundredths digit to ASCII, letter = letter+$30
         ADD  R0,R0,#0x30
;  17)output letter to the LCD by calling ST7735_OutChar
         BL  Nokia5110_OutChar
;  18)output " " to the LCD by calling ST7735_OutChar
         MOV R0,#' '
         BL  Nokia5110_OutChar
;  19)deallocate variables
outfix19
         ADD  SP,#8
;  20)restore the registers by pulling off the stack
         POP {R4,PC}
         ALIGN
errOutFix DCB "*.** ",0
         ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
