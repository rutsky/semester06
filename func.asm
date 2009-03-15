; test.asm
; Test block that will be appended to firmware end and runned from some point.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

    use32
        STMFD   SP!, {R0-R9,LR}
        ADD     R0, R0, R1
        ; Jumpl
        BL      000facb4h
        LDMFD   SP!, {R0-R9,PC}
