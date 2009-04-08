; fw_clear_screen.S
; Clearing screen function.
; Vladimir Rutsky <altsysrq@gmail.com>
; 08.04.2009

include 'resident_common.S'

; From RF-9000 disassembly.
clear_screen_func:
        cmp     r0, 0x0
        bne     skip_fw_address_loading
        ldr     r3, [pc, -8 + (frame_buffer_descr_real_addr - $)]
        ldr     r0, [r3]
      skip_fw_address_loading:
      
        ldmia   r0, {r3, r12}
        mul     r2, r12, r3
        
        nop
        
      loop:
        sub     r3, r2, 0x1
        mov     r2, r3
        cmn     r3, 0x1
        beq     func_end
        
        add     r3, r0, r2, lsl 1
        strh    r1, [r3, 0x8]
        b       loop
      
      func_end:
        mov     pc, lr
align 4
frame_buffer_descr_real_addr DW     fwFrameBufferVarAddr
