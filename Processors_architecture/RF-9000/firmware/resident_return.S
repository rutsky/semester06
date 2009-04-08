; resident_return.S
; Sample resident program: just returning function.
; Vladimir Rutsky <altsysrq@gmail.com>
; 01.04.2009

    USE32
        STMFD   SP!, {R0-R11,LR}
        LDMFD   SP!, {R0-R11,PC}
