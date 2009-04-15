; resident_draw.S
; Drawing demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.04.2009

include 'resident_common.S'

    USE32
    format binary
    
resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Calling `Draw2Lines' finction.
        MOV     R0, string1
        MOV     R1, string2

        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwDraw2LinesFunc_addr - $)]
        
        ; Doing something untill some key will be pressed.
loop:
        ; Calling `CheckYesNoKeys'.
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwCheckYesNoKeysFunc_addr - $)]
        
        MOV     R9, R0          ; saving result
        
        ; Calling `OSSleepFunc'.
        MOV     R0, 1
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwOSSleepFunc_addr - $)]
        
        ; Exiting if key was pressed.
        CMP     R9, 0
        BNE     resident_end
        
        B       loop
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}
        
fwCheckYesNoKeysFunc_addr    DW     fwCheckYesNoKeysFunc
fwOSSleepFunc_addr           DW     fwOSSleepFunc
fwDraw2LinesFunc_addr        DW     fwDraw2LinesFunc

                         ; aligning:  123 123 123 123 123
string1                      DB     'First',0,0,0
string2                      DB     'Second',0,0
