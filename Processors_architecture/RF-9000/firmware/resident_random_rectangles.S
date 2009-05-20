; resident_random_rectangles.S
; Buffered drawing of random rectangles example.
; Vladimir Rutsky <altsysrq@gmail.com>
; 20.05.2009

    USE32
    format binary

; Entry point.
entry_point:
        B       resident_start
; Enf of entry function

include 'resident_common.S'
include 'resident_random.S'
include 'resident_division.S'

resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Main loop.
loop: 
        ; Retrieving address of variable which stores adress of frame buffer structure.
        LDR     R4, [PC, -8 + (fwFrameBufferVar_addr - $)]
        ; Retrieving frame buffer structure address.
        LDR     R10, [R4]
        ; Loading width, height and actual frame buffer address.
        LDMIA   R10!, {R8, R9}
        
        ; Drawing one rectangle of random color.
        
        ; Generating rectangle bounds

        ;B        debug_l1 ; debug
        
        ; x1 (R4)
        BL      rand   ; R0 = rand()
        MOV     R1, R8 ; R1 = width
        BL      unsigned_integral_division
        MOV     R4, R1 ; R4 = rand() % width

        ; x2 (R6)
        BL      rand   ; R0 = rand()
        MOV     R1, R8 ; R1 = width
        BL      unsigned_integral_division
        MOV     R6, R1 ; R6 = rand() % width

        ; y1 (R5)
        BL      rand   ; R0 = rand()
        MOV     R1, R9 ; R1 = height
        BL      unsigned_integral_division
        MOV     R5, R1 ; R5 = rand() % height

        ; y2 (R7)
        BL      rand   ; R0 = rand()
        MOV     R1, R9 ; R1 = height
        BL      unsigned_integral_division
        MOV     R7, R1 ; R7 = rand() % height
        
        ; if (!(x1 < x2))
        ;   swap(x1, x2)
        CMP     R4, R6
        BLT     pass1 ; TODO
          MOV     R0, R4
          MOV     R4, R6
          MOV     R6, R0
        pass1:

        ; if (!(y1 < y2))
        ;   swap(y1, y2)
        CMP     R5, R7
        BLT     pass2 ; TODO
          MOV     R0, R5
          MOV     R5, R7
          MOV     R7, R0
        pass2:
        
  debug_l1: ; debug
  
        ; debug
        ;   MOV     R4, 50
        ;   MOV     R5, 30
        ;MOV     R6, 70
        ;   MOV     R7, 80
        ; end of debug
        
        ; x2 (R6)
        ;   MOV     R1, R8 ; R1 = width
        ;MOV     R1, 100 ; R1 = width ; debug
        ;   BL      rand   ; R0 = rand()
        ;MOV     R0, 1024 ; debug
        ;   BL      unsigned_integral_division
        ;MOV     R0, 10 ; debug
        ;MOV     R1, 24 ; debug
        ;   MOV     R6, R1 ; R7 = rand() % width
           
        ;CMP     R4, R6
        ;BLT     pass1d ; TODO
        ;  MOV     R0, R4
        ;  MOV     R4, R6
        ;  MOV     R6, R0
        ;pass1d:
  
        ; Generating color using rand().
        BL      rand
        MOV     R11, R0                                         ; R11 == current color

        MOV     R0, R5                                          ; R0 == row number

        loop_row:
        ;{
          MOV     R1, R4                                          ; R1 == column number
          
          loop_column:
          ;{
            ; R3 = pixel position in frame buffer
            MUL     R2, R0, R8
            ADD     R2, R2, R1
            ADD     R3, R10, R2, LSL 1
          
            ; Putting pixel.
            STRH    R11, [R3] ; TODO: increment can be done here.
            
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
        STMFD   SP!, {R0-R8}
          ; Calling `CheckYesNoKeys'.
          MOV     LR, PC
          LDR     PC, [PC, -8 + (fwCheckYesNoKeysFunc_addr - $)]
        
          MOV     R9, R0          ; saving result
        
          ; Calling `RealOSSleepFunc'.
          MOV     R0, 1
          MOV     LR, PC
          LDR     PC, [PC, -8 + (fwOSSleepFunc_addr - $)]
        LDMFD   SP!, {R0-R8}
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
