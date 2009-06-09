; motion_blur_05a64.asm
; Motion blur effect.
; Implementation 4 rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 64-bit Linux version.
;

global _motion_blur_apply_05_64

_motion_blur_apply_05_64:
        push    rbp 
        mov     rbp, rsp 
        
        
        
        leave                   ; mov esp,ebp / pop ebp 
        ret
