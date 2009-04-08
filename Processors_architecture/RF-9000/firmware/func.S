; func.S
; Test block that will be appended to firmware end and runned from some point.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Function is called instead of this commands:
;RAM:20009D44                 BL      PutString
;RAM:20009D48                 MOV     R3, #5          ; <--+
;RAM:20009D4C                 MOV     R2, #1          ;    | 
;RAM:20009D50                 LDR     R1, =0xF81F     ;    |  jump code will be inserted all here
;RAM:20009D54                 LDR     R0, =0xFE5B     ;    +---
;RAM:20009D58                 ADD     R4, SP, #0x37A8+var_37A0
;RAM:20009D5C                 STMIA   R4, {R0-R3}
;RAM:20009D60                 ADR     R3, aFreeMem    ; "FREE MEM : "

    USE32
    format binary
    
    startAddr     = 0x200FA600
    putStringAddr = 0x20044D28
func:
        ; Saving process state.
        STMFD   SP!, {R0-R11,LR}
        SUB     SP, SP, 0x1C00
        SUB     SP, SP, 0x184
        
        ; Body

        ; Trying do PutString on place of firmware version.
;        MOV     R3, 5
;        MOV     R2, 4
;        LDR     R1, [PC, r1_store - $ - 8]
;        LDR     R0, [PC, r1_store - $ - 8]
;        STMFA   SP, {R0-R3}
;        MOV     R3, msg
;        STR     R3, [SP]
;        MOV     R3, 0xDC
;        MOV     R2, 0x42
;        MOV     R1, 0
;        MOV     R0, 0
;        BL      putStringAddr - (startAddr + ($ - func))
        
loop:   B       loop ; Checking that this code is actually runs
        
        ; Restoring process state.
        MOV     R3, 5
        MOV     R2, 1
        LDR     R1, [PC, r1_store - $ - 8]
        LDR     R0, [PC, r0_store - $ - 8]
        ADD     SP, SP, 0x1C00
        ADD     SP, SP, 0x194
        LDMFD   SP!, {R4-R11,PC}

start      DW     0x0FA600
r1_store   DW     0x0F81F
r0_store   DW     0x0FE5B
msg        DB     'Hello world!',10
