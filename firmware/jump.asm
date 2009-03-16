; jump.asm
; Jump command for entering test function.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Hacking this part of code:
;RAM:20009D14                 BL      PutString
;RAM:20009D18                 MOV     R3, #5
;RAM:20009D1C                 MOV     R2, #4
;RAM:20009D20                 LDR     R1, =0xF81F
;RAM:20009D24                 LDR     R0, =0xFE5B
;RAM:20009D28                 STMFA   SP, {R0-R3}
;RAM:20009D2C                 ADR     R3, aVersion1_15 ; "Version 1.15"
;RAM:20009D30                 STR     R3, [SP,#0x37A8+var_37A8]
;RAM:20009D34                 MOV     R3, #0xDC
;RAM:20009D38                 MOV     R2, #0x42
;RAM:20009D3C                 MOV     R1, #0
;RAM:20009D40                 MOV     R0, #0
;RAM:20009D44                 BL      PutString
;RAM:20009D48                 MOV     R3, #5          ; <--+---
;RAM:20009D4C                 MOV     R2, #1          ;    | jump code will be inserted here
;RAM:20009D50                 LDR     R1, =0xF81F     ;    |
;RAM:20009D54                 LDR     R0, =0xFE5B     ;    +---
;RAM:20009D58                 ADD     R4, SP, #0x37A8+var_37A0
;RAM:20009D5C                 STMIA   R4, {R0-R3}
;RAM:20009D60                 ADR     R3, aFreeMem    ; "FREE MEM : "




; Position of our function is RAM:200FACB4 (one byte after firmware end, due to align).

    USE32
        BL      200FACB4h-20009D48h
