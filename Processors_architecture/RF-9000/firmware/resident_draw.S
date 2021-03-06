; resident_draw.S
; Drawing demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 01.04.2009

include 'resident_common.S'

    USE32
    format binary
    
    FWOffset               = 0xF95C

    RealFrameBufferVarAddr = 0x200FEA64
    RealCheckYesNoKeysFunc = 0x200237A0 + FWOffset
    RealOSSleepFunc        = 0x20090D64 + FWOffset
    RealDrawTextCenterFunc = 0x20044D28 + FWOffset
    RealOSTimerFunc        = 0x2003FCDC + FWOffset

resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Debug.
        LDR     R0, [PC, -8 + (some_color - $)]
        STRH    R0, [SP]
        ; End of debug.


loop: ; TODO

        ; Calling `ClearScreen' function.
        LDR     R0, [PC, -8 + (black_color - $)]
        LDR     R1, [SP]
        ADD     R1, R0, 1
        AND     R1, R1, R0
        STR     R1, [SP]
        
        MOV     R0, 0

        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwClearScreenFunc_addr - $)]
        ; End of 'ClearScreen' calling.

        ; Loading frame buffer information
        LDR     R5, [PC, -8 + (frame_buffer_descr_real_addr - $)]
        
        ; TODO: Use LDMIA R5, {R6, R7}
        ;LDR     R6, [R5, 0]     ; R6 = width
        ;LDR     R7, [R5, 4]     ; R7 = height
        ; debug
        MOV     R6, 100
        MOV     R7, 100
        
        ; Set R1 to real frame buffer.
        ADD     R5, R5, 8
        
        ; Putting pixel. DEBUG
        LDR     R0, [PC, -8 + (white_color - $)]
        LDR     R1, [PC, -8 + (black_color - $)]
        STRH    R0, [R5,  0] 
        STRH    R1, [R5,  2] 
        STRH    R0, [R5,  4] 
        STRH    R1, [R5,  6] 
        STRH    R0, [R5,  8] 
        STRH    R1, [R5, 10] 
        STRH    R0, [R5, 12] 
        STRH    R1, [R5, 14] 
        ; End of debug
        
        ; Drawing something on screen till some key will not be pressed.
        
;loop: ; TODO
        MOV     R3, R5                                          ; R3 == position in frame buffer
        
        MOV     R0, 0                                           ; R0 == row number
loop_row:
        
        MOV     R1, 0                                           ; R1 == column number
loop_column:
        
        ; Calculating `color'.
        MUL     R4, R0, R6
        ADD     R4, R4, R1
        
        ; Forcing color. DEBUG
        LDR     R4, [PC, -8 + (some_color - $)]
        
        ; Putting pixel.
        STRH    R4, [R3] ; TODO: increment can be done here.
        
        ADD     R3, R3, 2
        
        ADD     R1, R1, 1
        CMP     R1, R6
        BLT     loop_column                                     ; end of loop_column
        
        ADD     R0, R0, 1
        CMP     R0, R7
        BLT     loop_row                                        ; end of loop_row
        
        ;B       resident_end
        
        ; Checking keys state.
        ;STMFD   SP!, {R0-R7}
        
        ; Calling `CheckYesNoKeys'.
        MOV     LR, PC
        LDR     PC, [PC, -8 + (RealCheckYesNoKeysFunc_addr - $)]
        
        MOV     R9, R0          ; saving result
        
        ; Calling `RealOSSleepFunc'.
        MOV     R0, 1
        MOV     LR, PC
        LDR     PC, [PC, -8 + (RealOSSleepFunc_addr - $)]
        
        ;LDMFD   SP!, {R0-R7}
        
        ; Exiting if key was pressed.
        CMP     R9, 0
        BNE     resident_end
        
        ; Drawing `Hello' string.
        ;MOV     R3, 5
        ;MOV     R2, 4
        ;LDR     R1, [PC, -8 + (foreground_color - $)]
        ;LDR     R0, [PC, -8 + (background_color - $)]
        ;STMFA   SP, {R0-R3}                                     ; this requires memory on stack
        ;ADD     R3, PC, -8 + (hello_string - $)
        ;STR     R3, [SP]
        ;MOV     R3, 0xDC
        ;MOV     R2, 0x2E
        ;MOV     R1, 0
        ;MOV     R0, 0

        ;MOV     LR, PC
        ;LDR     PC, [PC, -8 + (RealDrawTextCenterFunc_addr - $)]
        ; End of string drawing.
        
        MOV     R0, 0
        MOV     LR, PC
        LDR     PC, [PC, -8 + (RealOSTimerFunc_addr - $)]
        
        B      loop
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}
        
                         ; aligning:  123 123 123 123 123
frame_buffer_descr_real_addr DW     RealFrameBufferVarAddr
RealCheckYesNoKeysFunc_addr  DW     RealCheckYesNoKeysFunc
RealOSSleepFunc_addr         DW     RealOSSleepFunc
RealDrawTextCenterFunc_addr  DW     RealDrawTextCenterFunc
RealOSTimerFunc_addr         DW     RealOSTimerFunc
fwClearScreenFunc_addr       DW     fwClearScreenFunc

; TODO: May be in other order.
foreground_color             DW     0x0F81F
background_color             DW     0x0FE5B
hello_string                 DB     'Hello!',0,0
test_string                  DB     'Test',0,0,0,0

some_color                   DW     0xAAAA
black_color                  DW     0x0000
white_color                  DW     0xFFFF
