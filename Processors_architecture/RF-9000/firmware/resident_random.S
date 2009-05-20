; resident_random.S
; Linear congruential pseudo-random number generator.
; Vladimir Rutsky <altsysrq@gmail.com>
; 20.05.2009

  ; Pseudorandom 16 bit number generator.
  ;   Next = (a * Seed + c) mod b
  ; Note: high 16 bits of generated number are better than lower 16.
rand:
        STMFD   SP!, {R1,R2,LR}
        
        LDR     R0, [PC, -8 + (rand_seed - $)]                  ; R0 = seed
        LDR     R1, [PC, -8 + (rand_a - $)]                     ; R1 = a
        MUL     R2, R0, R1                                      ; R2 = R0 * R = seed * a
        LDR     R1, [PC, -8 + (rand_c - $)]                     ; R1 = c
        ADD     R0, R2, R1                                      ; R0 = R2 + R1 = seed * a + c
        STR     R0, [PC, -8 + (rand_seed - $)]                  ; saving seed
        
        ; Post operations.
        MOV     R0, R0, LSR 16
        
        LDMFD   SP!, {R1,R2,PC}

rand_seed                    DW     0
rand_a                       DW     69069
rand_c                       DW     5
