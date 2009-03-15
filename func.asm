; test.asm
; Test block that will be appended to firmware end and runned from some point.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Function is called instead of this commands:
;RAM:20009D0C                 MOV     R1, #0     ; <--+
;RAM:20009D10                 MOV     R0, #0     ;    | Code will be inserted here

    use32
        STMFD   SP!, {R2-R9,LR}
        ; Jumpl
        MOV     R0, 0
        MOV     R1, 0
        LDMFD   SP!, {R2-R9,PC}
