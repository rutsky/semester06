; resident_draw.asm
; Drawing demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 01.04.2009

    USE32
    format binary

    RealFrameBufferVarAddr = 0x200FEA64
    RealCheckYesNoKeysFunc = 0x200237A0
    RealOSSleepFunc        = 0x20090D64

resident_start:
        STMFD   SP!, {R0-R11,LR}
        

        ; Loading frame buffer information
        LDR     R5, [PC, frame_buffer_descr_real_addr - $ - 8]
        
        ; TODO: Use LDMIA R5, {R6, R7}
        LDR     R6, [R5, 0]     ; R6 = width
        LDR     R7, [R0, 4]     ; R7 = height
        
        ; Set R1 to real frame buffer.
        ADD     R5, R5, 8
        
        ; Drawing something on screen till some key will not be pressed.
        
loop:
        MOV     R0, 0           ; R0 == row number
loop_row:
        
        MOV     R1, 0           ; R1 == column number
        
        MOV     R3, R5          ; R3 == position in frame buffer
loop_column:
        
        ; Calculating `color'.
        MUL     R4, R0, R6
        ADD     R4, R4, R1
        
        ; Putting pixel.
        STRH    R4, [R3]
        
        ADD     R3, R3, 2
        
        ADD     R1, R1, 1
        CMP     R1, R6
        BLT     loop_column
        
        ADD     R0, R0, 1
        CMP     R0, R7
        BLT     loop_row
        
        ; Checking keys state.
        STMFD   SP!, {R0-R7}
        
        ; Calling `CheckYesNoKeys'.
        MOV     LR, PC
        LDR     PC, [PC, RealCheckYesNoKeysFunc_addr - $ - 8]
        
        MOV     R9, R0          ; saving result
        
        ; Calling `RealOSSleepFunc'.
        MOV     R0, 1
        MOV     LR, PC
        LDR     PC, [PC, RealOSSleepFunc_addr - $ - 8]
        
        LDMFD   SP!, {R0-R7}
        
        ; Exiting if key was pressed.
        CMP     R9, 0
        BNE     resident_end
        
        B      loop
        
resident_end:
        LDMFD   SP!, {R0-R11,PC}
frame_buffer_descr_real_addr DW     RealFrameBufferVarAddr
RealCheckYesNoKeysFunc_addr  DW     RealCheckYesNoKeysFunc
RealOSSleepFunc_addr         DW     RealOSSleepFunc
