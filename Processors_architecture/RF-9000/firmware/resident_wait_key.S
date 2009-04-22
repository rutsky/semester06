; resident_wait_key.S
; Awaiting of key press example.
; Vladimir Rutsky <altsysrq@gmail.com>
; 22.04.2009

include 'resident_common.S'

    USE32
    format binary
    
resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Main loop.
loop: 
        
        ; Checking keys state.
        
        ; Calling `CheckYesNoKeys'.
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwCheckYesNoKeysFunc_addr - $)]
        
        MOV     R9, R0          ; saving result
        
        ; Calling `RealOSSleepFunc'.
        MOV     R0, 1
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwOSSleepFunc_addr - $)]
        
        ; Exiting if key was pressed.
        CMP     R9, 0
        BNE     resident_end
        
        B      loop
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}

fwOSSleepFunc_addr           DW     fwOSSleepFunc
fwCheckYesNoKeysFunc_addr    DW     fwCheckYesNoKeysFunc

                         ; aligning:  123 123 123 123 123
