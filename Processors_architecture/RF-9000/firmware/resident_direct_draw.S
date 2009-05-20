; resident_direct_draw.S
; Actually it is example on buffered drawing.
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
        ;LDR     R5, [PC, -8 + (fwFrameBufferVar_addr - $)]
        ;LDR     R6, [PC, -8 + (hwIMG_REG_BASE0_addr - $)]
        ;STR     R5, [R6]
        
        ; Retrieving address of variable which stores adress of frame buffer structure.
        ;LDR     R4, [PC, -8 + (hwIMG_REG_BASE0_addr - $)]
        LDR     R4, [PC, -8 + (fwFrameBufferVar_addr - $)]
        
        ; Retrieving frame buffer structure address.
        LDR     R5, [R4]
        
        ; Loading width, height and actual frame buffer address.
        LDMIA   R5!, {R8, R9}

        ; Loading colors.
        LDR     R0, [PC, -8 + (white_color - $)]
        LDR     R1, [PC, -8 + (black_color - $)]
        
        ; Putting some pixels.
        STRH    R0, [R5,  0]
        STRH    R1, [R5,  2]
        STRH    R0, [R5,  4]
        STRH    R1, [R5,  6]
        STRH    R0, [R5,  8]
        STRH    R1, [R5, 10]
        STRH    R0, [R5, 12]
        STRH    R1, [R5, 14]
        
        ; Trying is something will update real screen.
        ; Calling `SetPopupTimer'.
        MOV     R0, 0
        MOV     LR, PC
        LDR     PC, [PC, -8 + (fwSetPopupTimerFunc_addr - $)]
        
        ; Checking keys state.
        STMFD   SP!, {R0-R6}
          ; Calling `CheckYesNoKeys'.
          MOV     LR, PC
          LDR     PC, [PC, -8 + (fwCheckYesNoKeysFunc_addr - $)]
        
          MOV     R9, R0          ; saving result
        
          ; Calling `RealOSSleepFunc'.
          MOV     R0, 1
          MOV     LR, PC
          LDR     PC, [PC, -8 + (fwOSSleepFunc_addr - $)]
        LDMFD   SP!, {R0-R6}
        ; End of keys state checking
        
        ; Exiting if key was pressed.
        CMP     R9, 0
        BNE     resident_end
        
        B      loop
        
resident_end:
        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}

fwOSSleepFunc_addr           DW     fwOSSleepFunc
fwCheckYesNoKeysFunc_addr    DW     fwCheckYesNoKeysFunc
fwSetPopupTimerFunc_addr     DW     fwSetPopupTimerFunc

fwFrameBufferVar_addr        DW     fwFrameBufferVarAddr
hwIMG_REG_BASE0_addr         DW     hwIMG_REG_BASE0

some_color                   DW     0xAAAA
black_color                  DW     0x0000
white_color                  DW     0xFFFF

                         ; aligning:  123 123 123 123 123
