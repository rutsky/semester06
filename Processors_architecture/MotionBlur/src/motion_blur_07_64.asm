; motion_blur_07_64.asm
; Motion blur effect.
; Implementation 6 rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 64-bit Linux version.
;

global _motion_blur_apply_07_64

; void _motion_blur_apply_07_64(
;        byte_type *image,                    // rdi
;        int w,                               // rsi
;        int h,                               // rdx
;        int scanlineLen,                     // rcx
;        byte_type const *background,         // r8
;        int nMovingLayers,                   // r9
;        byte_type const *const *movingLayers // QWORD [rbp + 8 * 2]
;      );

%define nLocalVars    20

; Arguments.
%define image           rdi
%define w               esi
%define arg_h           edx
%define arg_scanlineLen ecx
%define background      r8
%define nMovingLayers   r9d
%define movingLayers    QWORD [rbp + 8 * 2]

%define h                    DWORD [rbp - 8 *  1]
%define scanlineLen          DWORD [rbp - 8 *  2]

; Local variables.
%define invNMovingLayers     DWORD [rbp - 8 *  3]
%define y                    DWORD [rbp - 8 *  4]
%define x                    DWORD [rbp - 8 *  5]
%define idx                  DWORD [rbp - 8 *  6]
%define lastLayerMovingPixel DWORD [rbp - 8 *  7]
%define backgroundPixelR     DWORD [rbp - 8 *  8]
%define backgroundPixelG     DWORD [rbp - 8 *  9]
%define backgroundPixelB     DWORD [rbp - 8 * 10]
%define totalR               DWORD [rbp - 8 * 11]
%define totalG               DWORD [rbp - 8 * 12]
%define totalB               DWORD [rbp - 8 * 13]

        section .text

_motion_blur_apply_07_64:
        push    rbp                  ; saving previous rbp
        mov     rbp, rsp             ; moving into current rbp rsp: base for arguments and variables
        
        sub     rsp, 8 * nLocalVars  ; allocating memory on stack for local variables
        
        push    rax                  ; saving registers, just to be sure
        push    rbx
        
        ; Reorganizing input variables.
        mov     h, arg_h
        mov     scanlineLen, arg_scanlineLen
        
        ; Calculating `1/nMovingLayers'.
        mov     eax, (1 << 8)
        cdq ; magic with edx, in other case idiv will spawn FPU exception.
        idiv    nMovingLayers
        
        ; Cloning `1/nMovingLayers' into 4 mm5 words.
        movd    mm5, eax
        pshufw  mm5, mm5, 0
        
        ; Storing zeroes at mm6.
        movq    mm6, QWORD [zeroes]
        
        ; Storing mask.
        movq    mm7, QWORD [mask]
        
        mov     y, 0
        ; Loop by y.
  .loop_by_y:
        mov     eax, y
        mov     ebx, h
        cmp     eax, ebx
        jge     .end_loop_by_y
        
        mov     x, 0
        ; Loop by x.
  .loop_by_x:
        mov     eax, x
        mov     ebx, w
        cmp     eax, ebx
        jge     .end_loop_by_x
        
        ; Now working with (x, y) pixel.
        
        ; Calculating index (in edx).
        mov     eax, y
        imul    eax, scanlineLen
        mov     edx, x
        shl     edx, 2
        add     edx, eax
        
        ; Loading last layer pixel (in ecx).
        mov     eax, nMovingLayers
        dec     eax
        shl     eax, 3     ; 3 == log(2, sizeof(byte_type *))
        add     rax, movingLayers
        mov     rax, [rax] ; retrieved last layer start address
        add     rax, rdx
        mov     ecx, DWORD [rax]
        
        ; Checking is pixel value has alpha (in ARGB model it means that pixel value > 0xffffff).
        cmp     ecx, 0x00ffffff
        jbe     .last_layer_transparent
        
  ;last_layer_not_transparent:
        ; Writing last layer pixel on image.
        mov     rax, image
        add     rax, rdx
        mov     DWORD [rax], ecx
        
        jmp     .last_layer_transparent_if_end
        
  .last_layer_transparent:
        ; Loading background pixel into mm0 (packed as words).
        mov     rax, background
        add     rax, rdx
        movd    mm0, DWORD [rax]
        punpcklbw mm0, mm6
        
        ; mm4 will accumulate color sum.
        movq    mm4, mm0
        
        ; Iterating through layers.
        mov     ecx, nMovingLayers
        dec     ecx
  .next_layer:
        ; Loading layer pixel (in ebx).
        mov     eax, ecx
        dec     eax
        shl     eax, 3     ; 3 == log(2, sizeof(byte_type *))
        add     rax, movingLayers
        mov     rax, [rax] ; retrieved layer start address
        add     rax, rdx
        mov     ebx, DWORD [rax]
        
        ; Checking is pixel value has alpha.
        cmp     ebx, 0x00ffffff
        jbe     .layer_dont_have_alpha
        
        ; Adding layer pixel value to accumulators.
        
        ; Loading moving layer pixel into mm1 (packed as words).
        movd    mm1, ebx
        punpcklbw mm1, mm6
        
        ; mm4 += mm1
        paddsw  mm4, mm1
        
        jmp     .layer_dont_have_alpha_if_end
        
  .layer_dont_have_alpha:
        ; Adding to colors accumulators background pixel value.
        paddsw    mm4, mm0
  .layer_dont_have_alpha_if_end:
        
        dec     ecx
        jnz     .next_layer
        
        ; Writing pixel average value on image.

        mov     rcx, image
        add     rcx, rdx
        
        ; mm4 = mm4 * ((1 << 8)/nMovingLayers)
        pmullw   mm4, mm5
        ; mm4 = mm4 >> 8
        psrlw    mm4, 8
        ; mm4 = mm4 & 0xff
        pand     mm4, mm7
        ; Packing result with saturation.
        packuswb mm4, mm7
        
        ; Writing.
        movd     DWORD [rcx], mm4
  .last_layer_transparent_if_end:
  
        mov     eax, x
        inc     eax
        mov     x, eax
        jmp     .loop_by_x
  .end_loop_by_x:
        ; End of loop by x.
        
        mov     eax, y
        inc     eax
        mov     y, eax
        jmp     .loop_by_y
        
  .end_loop_by_y:
        ; End of loop by y.
  
        ; Restoring FPU registers.
        emms
  
        pop     rbx                  ; restoring registers
        pop     rax
  
        add     rsp, 8 * nLocalVars  ; restoring stack state
        leave                        ; mov esp,ebp / pop ebp 
        ret

        section .data
zeroes: db      0,0,0,0, 0,0,0,0
mask:   db      0xff,0x00,0xff,0x00, 0xff,0x00,0xff,0x00
