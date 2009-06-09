; motion_blur_07_32.asm
; Motion blur effect.
; Implementation 6 rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 32-bit version.
;

[bits 32]
[section .bss align=16]
[section .data align=16]
[section .text align=16]
[global _motion_blur_apply_07_32]

; void _motion_blur_apply_07_32(
;        byte_type *image,                    // DWORD [ebp + 4 * 2]
;        int w,                               // DWORD [ebp + 4 * 3]
;        int h,                               // DWORD [ebp + 4 * 4]
;        int scanlineLen,                     // DWORD [ebp + 4 * 5]
;        byte_type const *background,         // DWORD [ebp + 4 * 6]
;        int nMovingLayers,                   // DWORD [ebp + 4 * 7]
;        byte_type const *const *movingLayers // DWORD [ebp + 4 * 8]
;      );

%define nLocalVars    20

; Arguments.
%define image           DWORD [ebp + 4 * 2]
%define w               DWORD [ebp + 4 * 3]
%define h               DWORD [ebp + 4 * 4]
%define scanlineLen     DWORD [ebp + 4 * 5]
%define background      DWORD [ebp + 4 * 6]
%define nMovingLayers   DWORD [ebp + 4 * 7]
%define movingLayers    DWORD [ebp + 4 * 8]

; Local variables.
%define invNMovingLayers     DWORD [ebp - 4 *  1]
%define y                    DWORD [ebp - 4 *  2]
%define x                    DWORD [ebp - 4 *  3]
%define idx                  DWORD [ebp - 4 *  4]
%define lastLayerMovingPixel DWORD [ebp - 4 *  5]
%define backgroundPixelR     DWORD [ebp - 4 *  6]
%define backgroundPixelG     DWORD [ebp - 4 *  7]
%define backgroundPixelB     DWORD [ebp - 4 *  8]
%define totalR               DWORD [ebp - 4 *  9]
%define totalG               DWORD [ebp - 4 * 10]
%define totalB               DWORD [ebp - 4 * 11]

        section .text

_motion_blur_apply_07_32:
        push    ebp                  ; saving previous ebp
        mov     ebp, esp             ; moving into current ebp esp: base for arguments and variables
        
        sub     esp, 4 * nLocalVars  ; allocating memory on stack for local variables
        
        push    rax                  ; saving registers, just to be sure
        push    ebx
        
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
        shl     eax, 2     ; 2 == log(2, sizeof(byte_type *))
        add     eax, movingLayers
        mov     eax, [eax] ; retrieved last layer start address
        add     eax, edx
        mov     ecx, DWORD [eax]
        
        ; Checking is pixel value has alpha (in ARGB model it means that pixel value > 0xffffff).
        cmp     ecx, 0x00ffffff
        jbe     .last_layer_transparent
        
  ;last_layer_not_transparent:
        ; Writing last layer pixel on image.
        mov     eax, image
        add     eax, edx
        mov     DWORD [eax], ecx
        
        jmp     .last_layer_transparent_if_end
        
  .last_layer_transparent:
        ; Loading background pixel into mm0 (packed as words).
        mov     eax, background
        add     eax, edx
        movd    mm0, DWORD [eax]
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
        shl     eax, 2     ; 2 == log(2, sizeof(byte_type *))
        add     eax, movingLayers
        mov     eax, [eax] ; retrieved layer start address
        add     eax, edx
        mov     ebx, DWORD [eax]
        
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

        mov     ecx, image
        add     ecx, edx
        
        ; mm4 = mm4 * ((1 << 8)/nMovingLayers)
        pmullw   mm4, mm5
        ; mm4 = mm4 >> 8
        psrlw    mm4, 8
        ; mm4 = mm4 & 0xff
        pand     mm4, mm7
        ; Packing result with saturation.
        packuswb mm4, mm7
        
        ; Writing.
        movd     DWORD [ecx], mm4
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
  
        pop     ebx                  ; restoring registers
        pop     eax
  
        add     esp, 4 * nLocalVars  ; restoring stack state
        leave                        ; mov esp,ebp / pop ebp 
        ret

        section .data
zeroes: db      0,0,0,0, 0,0,0,0
mask:   db      0xff,0x00,0xff,0x00, 0xff,0x00,0xff,0x00
