; test.asm
; Test block that will be appended to firmware end and runned from some point.
; Vladimir Rutsky <altsysrq@gmail.com>
; 15.03.2009

; Function is called instead of this commands:
;RAM:20009CAC                 MOV     R3, #0                        ; <-- Our code start.
;RAM:20009CB0                 MOV     R2, #1
;RAM:20009CB4                 MOV     R1, #0x93
;RAM:20009CB8                 MOV     R0, #0xDC
;RAM:20009CBC                 ADD     R4, SP, #0x37A8+var_37A0
;RAM:20009CC0                 STMIA   R4, {R0-R3}
;RAM:20009CC4                 MOV     R2, #0
;RAM:20009CC8                 LDR     R0, =0x204F9818
;RAM:20009CCC                 STMEA   SP, {R2,R3}
;RAM:20009CD0                 LDR     R1, =0xFFF8E308
;RAM:20009CD4                 SUB     R3, R0, R1
;RAM:20009CD8                 MOV     R2, #0x1D
;RAM:20009CDC                 MOV     R1, #0
;RAM:20009CE0                 MOV     R0, #0
;RAM:20009CE4                 BL      PutWString
;     ...
;RAM:20009F54                 MOV     R1, #0xA
;RAM:20009F58                 STR     R0, [SP,#0x37A8+var_3764]
;RAM:20009F5C                 ADD     R12, SP, #0x37A8+var_37A0
;RAM:20009F60                 STMIA   R12, {R0-R3}
;RAM:20009F64                 MOV     R2, #0
;RAM:20009F68                 STMEA   SP, {R2,R3}
;RAM:20009F6C                 LDR     R3, =0x20780B4C
;RAM:20009F70                 MOV     R2, #0x8C
;RAM:20009F74                 MOV     R1, #0x1A
;RAM:20009F78                 MOV     R0, #0
;RAM:20009F7C                 BL      PutWString
;RAM:20009F80                 NOP

;RAM:20009F84                 CMP     R7, #0                        ; <-- Jumping here at after our stuff.
;RAM:20009F88                 BEQ     loc_2000A120
;RAM:20009F8C                 BL      sub_20023CA4
;RAM:20009F90                 STR     R0, [SP,#0x37A8+var_3764]
;RAM:20009F94                 STR     R0, [SP,#0x37A8+var_3748]
;RAM:20009F98                 LDRB    R0, [SP,#0x37A8+var_3748]
;RAM:20009F9C                 CMP     R0, #6
;RAM:20009FA0                 BNE     loc_20009FAC
;RAM:20009FA4                 MOV     R7, #0
;RAM:20009FA8                 B       loc_20009FB4

    USE32
    format binary
    
    RealStartAddr      = 0x20009CAC
    RealEndAddr        = 0x20009F84

    ; Loading file contents from `CONFIG' directory function.
    ;   Input:
    ;     R0 - pointer to file name string
    ;     R1 - pointer to buffer
    ;     R2 - length
    ;   Result:
    ;     R0 >= 0, on success
    ; Function don't touch R4-R10 registers states.
    RealLoadConfigFuncAddr = 0x20021174
    
    ; Sample on this function can be found in code below.
    RealDrawTextCenterFuncAddr = 0x20044D28
    
start:
        ; Saving process state.
        STMFD   SP!, {R0-R11,LR}
        
        ;******************************************************
        ; Loading and executing code from file on flash drive.
        ;******************************************************

        StackAdditionalSpace = 16
        BlockSize            = 1024
        BlockStackOffset     = 8
        
        ; Allocating memory on stack.
        SUB     SP, SP, BlockSize + StackAdditionalSpace
        
        ; Setting file name to load from.
        ADD     R0, PC, file_name - $ - 8
        MOV     R6, R0                                 ; storing pointer to file name string in R6
        
        ; Setting buffer pointer
        ADD     R1, SP, BlockStackOffset               ; additional offset for some cases.
        MOV     R7, R1                                 ; storing pointer to loading block in R7
        
        ; Setting size of block to load.
        MOV     R2, BlockSize
        
        ; Loading block from file using firmware function.
        BL      $ + ((RealLoadConfigFuncAddr - RealStartAddr) - ($ - start))
        
        ; Checking is file loaded correctly.
        CMP     R0, 0
        BLT     loading_error
loading_success:
        ; File contents succesfull loaded.
        
        ; Running loaded code now.
        MOV     LR, PC ; saving return point
        MOV     PC, R7
        
        B       boot_ldr_end
        
loading_error:
        ; Failed to load file contents.

        ; Printing error message as done here:
        ;RAM:20009CE8                 MOV     R3, #5
        ;RAM:20009CEC                 MOV     R2, #4
        ;RAM:20009CF0                 LDR     R1, =0xF81F
        ;RAM:20009CF4                 LDR     R0, =0xFE5B
        ;RAM:20009CF8                 STMFA   SP, {R0-R3}
        ;RAM:20009CFC                 ADR     R3, aOct08200720571 ; "Oct 08 2007 20:57:16"
        ;RAM:20009D00                 STR     R3, [SP]
        ;RAM:20009D04                 MOV     R3, #0xDC
        ;RAM:20009D08                 MOV     R2, #0x2E
        ;RAM:20009D0C                 MOV     R1, #0
        ;RAM:20009D10                 MOV     R0, #0
        ;RAM:20009D14                 BL      DrawTextCenterFunc
        
        ; Allocating memory at stack.
        MemOnStack = 32
        SUB     SP, SP, MemOnStack
        
        ; Calling `DrawTextCenterFunc'.
        MOV     R3, 5
        MOV     R2, 4
        LDR     R1, [PC, print_text_smth1 - $ - 8]
        LDR     R0, [PC, print_text_smth0 - $ - 8]
        STMFA   SP, {R0-R3}                                       ; this requires memory on stack
        ADD     R3, PC, error_str - $ - 8
        STR     R3, [SP]
        MOV     R3, 0xDC
        MOV     R2, 0x2E
        MOV     R1, 0
        MOV     R0, 0
        BL      $ + ((RealDrawTextCenterFuncAddr - RealStartAddr) - ($ - start))
        
        ; Freeing memory on stack.
        ADD     SP, SP, MemOnStack

boot_ldr_end:
        ; Setting back stack pointer.
        ADD     SP, SP, BlockSize + StackAdditionalSpace
        ;******************************************************
        
;loop:   B       loop ; Checking that this code is actually runs
        NOP
        NOP
        NOP
        NOP
        NOP
        
        ; Restoring process state.
        LDMFD   SP!, {R0-R11,LR}
        B $ + ((RealEndAddr - RealStartAddr) - ($ - start))

file_name        DB     'MBOOTLDR.BIN',0,0,0,0
print_text_smth1 DW     0x0F81F
print_text_smth0 DW     0x0FE5B
error_str        DB     'Failed to load code.',0,0,0
