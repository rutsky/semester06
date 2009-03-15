; jump.asm
; Jump command for entering test function.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Hacking this part of code:
;RAM:20009CE8                 MOV     R3, #5
;RAM:20009CEC                 MOV     R2, #4
;RAM:20009CF0                 LDR     R1, =0xF81F
;RAM:20009CF4                 LDR     R0, =0xFE5B
;RAM:20009CF8                 STMFA   SP, {R0-R3}
;RAM:20009CFC                 ADR     R3, aOct08200720571 ; "Oct 08 2007 20:57:16"
;RAM:20009D00                 STR     R3, [SP]
;RAM:20009D04                 MOV     R3, #0xDC
;RAM:20009D08                 MOV     R2, #0x2E
;RAM:20009D0C                 MOV     R1, #0     ; <--+
;RAM:20009D10                 MOV     R0, #0     ;    | Code will be inserted here
;RAM:20009D14                 BL      PutString

; Position of our function is RAM:200FACB4 (one byte after firmware end, due to align).

    use32
        BL      200FACB4h-20009D0Ch
