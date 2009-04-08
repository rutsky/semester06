; resident_clear_screen.S
; Clearing screen demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 08.04.2009

include 'resident_common.S'

    USE32
    format binary

resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x0
        SUB     SP, SP, MemOnStack
        
        ; Calling `ClearScreen' function.
        MOV     R0, 0
        LDR     R1, [PC, -8 + (black_color - $)]
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwClearScreenFunc_addr - $)]
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}
        
frame_buffer_descr_real_addr DW     fwFrameBufferVarAddr
fwClearScreenFunc_addr       DW     fwClearScreenFunc

black_color                  DW     0x0000                      ; black
