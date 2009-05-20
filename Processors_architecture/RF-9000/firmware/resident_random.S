; resident_random.S
; Linear congruential pseudo-random number generator.
; Vladimir Rutsky <altsysrq@gmail.com>
; 20.05.2009

  ; Pseudorandom number generator.
  ;   Next = (a * Seed + c) mod b
rand:
        STMFD   SP!, {R1,R2,LR}
        
        LDR     R0, [PC, -8 + (seed - $)]                       ; R0 = seed
        LDR     R1, [PC, -8 + (a - $)]                          ; R1 = a
        MUL     R2, R0, R1                                      ; R2 = R0 * R = seed * a
        LDR     R1, [PC, -8 + (c - $)]                          ; R1 = c
        ADD     R0, R2, R1                                      ; R0 = R2 + R1 = seed * a + c
        STR     R0, [PC, -8 + (seed - $)]                       ; saving seed
        
        LDMFD   SP!, {R1,R2,PC}

seed                         DW     0
a                            DW     69069
c                            DW     5
