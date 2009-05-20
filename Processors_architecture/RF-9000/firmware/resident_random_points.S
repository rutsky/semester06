; resident_random_points.S
; Buffered drawing of random points example.
; Vladimir Rutsky <altsysrq@gmail.com>
; 20.05.2009

include 'resident_common.S'

    USE32
    format binary
    
resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Main loop.
loop: 
        ; Retrieving address of variable which stores adress of frame buffer structure.
        LDR     R4, [PC, -8 + (fwFrameBufferVar_addr - $)]
        
        ; Retrieving frame buffer structure address.
        LDR     R5, [R4]
        
        ; Loading width, height and actual frame buffer address.
        LDMIA   R5!, {R6, R7}

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
        
        MOV     R3, R5                                          ; R3 == position in frame buffer
        MOV     R0, 0                                           ; R0 == row number
        
        loop_row:
        ;{
          MOV     R1, 0                                           ; R1 == column number
          
          loop_column:
          ;{

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
          ;}

          ADD     R0, R0, 1
          CMP     R0, R7
          BLT     loop_row                                        ; end of loop_row
        ;}
        
        ; Calling `SetPopupTimer' -- flushing screen frame buffer.
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
