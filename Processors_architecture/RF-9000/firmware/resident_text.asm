; resident_text.asm
; Drawing text demo.
; Vladimir Rutsky <altsysrq@gmail.com>
; 01.04.2009

    USE32
    format binary

    RealDrawTextCenterFunc = 0x20044D28

resident_start:
        STMFD   SP!, {R0-R11,LR}
        MemOnStack = 0x20
        SUB     SP, SP, MemOnStack
        
        ; Drawing `Hello' string.
        MOV     R3, 5
        MOV     R2, 4
        LDR     R1, [PC, foreground_color - $ - 8]
        LDR     R0, [PC, background_color - $ - 8]
        STMFA   SP, {R0-R3}                                       ; this requires memory on stack
        ADD     R3, PC, hello_string - $ - 8
        STR     R3, [SP]
        MOV     R3, 0xDC
        MOV     R2, 0x2E
        MOV     R1, 0
        MOV     R0, 0
        
        MOV     LR, PC
        LDR     PC, [PC, RealDrawTextCenterFunc_addr - $ - 8]

        ADD     SP, SP, MemOnStack
        LDMFD   SP!, {R0-R11,PC}
RealDrawTextCenterFunc_addr  DW     RealDrawTextCenterFunc

; TODO: May be in other order.
foreground_color             DW     0x0F81F
background_color             DW     0x0FE5B
hello_string                 DB     'Hello!',0,0
