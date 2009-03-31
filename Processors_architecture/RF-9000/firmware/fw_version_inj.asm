; test.asm
; Test block that will be appended to firmware end and runned from some point.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Function is called instead of this commands:
;RAM:20009CAC                 MOV     R3, #0                        ; <-- Our code start.
;RAM:20009CB0                 MOV     R2, #1
;RAM:20009CB4                 MOV     R1, #0x93
;RAM:20009CB8                 MOV     R0, #0xDC
;RAM:20009CBC                 ADD     R4, SP, #0x37A8+var_37A0
;RAM:20009CC0                 STMIA   R4, {R0-R3}
;RAM:20009CC4                 MOV     R2, #0
;RAM:20009CC8                 LDR     R0, =0x204F9818
;RAM:20009CCC                 STMEA   SP, {R2,R3}
;RAM:20009CD0                 LDR     R1, =0xFFF8E308
;RAM:20009CD4                 SUB     R3, R0, R1
;RAM:20009CD8                 MOV     R2, #0x1D
;RAM:20009CDC                 MOV     R1, #0
;RAM:20009CE0                 MOV     R0, #0
;RAM:20009CE4                 BL      PutWString
;     ...
;RAM:20009F54                 MOV     R1, #0xA
;RAM:20009F58                 STR     R0, [SP,#0x37A8+var_3764]
;RAM:20009F5C                 ADD     R12, SP, #0x37A8+var_37A0
;RAM:20009F60                 STMIA   R12, {R0-R3}
;RAM:20009F64                 MOV     R2, #0
;RAM:20009F68                 STMEA   SP, {R2,R3}
;RAM:20009F6C                 LDR     R3, =0x20780B4C
;RAM:20009F70                 MOV     R2, #0x8C
;RAM:20009F74                 MOV     R1, #0x1A
;RAM:20009F78                 MOV     R0, #0
;RAM:20009F7C                 BL      PutWString
;RAM:20009F80                 NOP

;RAM:20009F84                 CMP     R7, #0                        ; <-- Jumping here at after our stuff.
;RAM:20009F88                 BEQ     loc_2000A120
;RAM:20009F8C                 BL      sub_20023CA4
;RAM:20009F90                 STR     R0, [SP,#0x37A8+var_3764]
;RAM:20009F94                 STR     R0, [SP,#0x37A8+var_3748]
;RAM:20009F98                 LDRB    R0, [SP,#0x37A8+var_3748]
;RAM:20009F9C                 CMP     R0, #6
;RAM:20009FA0                 BNE     loc_20009FAC
;RAM:20009FA4                 MOV     R7, #0
;RAM:20009FA8                 B       loc_20009FB4

    USE32
    format binary
    
    realStartAddr = 0x20009CAC
    realEndAddr   = 0x20009F84
start:
        ; Saving process state.
        STMFD   SP!, {R0-R11,LR}
        
;loop:   B       loop ; Checking that this code is actually runs
        NOP
        NOP
        NOP
        NOP
        NOP
        
        ; Restoring process state.
        LDMFD   SP!, {R0-R11,LR}
        B $ + ((realEndAddr - realStartAddr) - ($ - start))
