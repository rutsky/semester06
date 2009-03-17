; jump.asm
; Jump command for entering test function.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Hacking this part of code:
;RAM:20009B80                 STMFD   SP!, {R0,R4-R11,LR}      <-- injecting at this line
;RAM:20009B84                 SUB     SP, SP, #0x3780
;RAM:20009B88                 ADD     R6, SP, #0x37A8+var_3734
;RAM:20009B8C                 MOV     R0, #0
;RAM:20009B90                 STR     R0, [SP,#0x37A8+var_373C]
;RAM:20009B94                 MOV     R7, #1
;RAM:20009B98                 MOV     R0, #1
;RAM:20009B9C                 STR     R0, [SP,#0x37A8+var_3740]
;RAM:20009BA0                 MOV     R0, #0
;RAM:20009BA4                 STR     R0, [SP,#0x37A8+var_3744]
;RAM:20009BA8                 MOV     R0, #1

    USE32
        STMFD   SP!, {LR}
        LDMFD   SP!, {PC}
