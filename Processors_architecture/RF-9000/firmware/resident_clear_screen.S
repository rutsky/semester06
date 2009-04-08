; resident_clear_screen.S
; Clearing screen demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 08.04.2009

    USE32
    format binary
    
    FWOffset               = 0xF95C

    RealFrameBufferVarAddr = 0x200FEA64
    RealClearScreenFunc    = 0x2040174

resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x0
        SUB     SP, SP, MemOnStack
        
        ; Calling `ClearScreen' function.
        MOV     LR, PC
        LDR     PC, [PC, -8 + (RealClearScreenFunc_addr - $)]
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}
        
frame_buffer_descr_real_addr DW     RealFrameBufferVarAddr
RealClearScreenFunc_addr     DW     RealClearScreenFunc
