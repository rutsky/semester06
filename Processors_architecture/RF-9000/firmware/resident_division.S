; resident_division.S
; Simple unsigned integral division.
; Vladimir Rutsky <altsysrq@gmail.com>
; 20.05.2009

  ; Simple unsigned integral division.
  ; Result:
  ;   R0 = R0 / R1
  ;   R1 = R0 % R1
unsigned_integral_division:
        STMFD   SP!, {R2,LR}
        
        MOV     R2, 0                                           ; division result
        
  division_loop:
        CMP     R0, R1
        BLT     division_end
        
        CMP     R0, 0
        BEQ     division_end
        
        ADD     R2, R2, 1
        SUB     R0, R0, R1
        
        B       division_loop

  division_end:
        MOV     R1, R0
        MOV     R0, R2

        LDMFD   SP!, {R2,PC}
