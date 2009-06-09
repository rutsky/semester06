; motion_blur_05_32.asm
; Motion blur effect.
; Implementation 4 rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 32-bit version.
;

[bits 32]
[section .bss align=16]
[section .data align=16]
[section .text align=16]
[global _motion_blur_apply_05_32]

; void _motion_blur_apply_05_32(
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
%define invNMovingLayers     DWORD [ebp - 4 *  3]
%define y                    DWORD [ebp - 4 *  4]
%define x                    DWORD [ebp - 4 *  5]
%define idx                  DWORD [ebp - 4 *  6]
%define lastLayerMovingPixel DWORD [ebp - 4 *  7]
%define backgroundPixelR     DWORD [ebp - 4 *  8]
%define backgroundPixelG     DWORD [ebp - 4 *  9]
%define backgroundPixelB     DWORD [ebp - 4 * 10]
%define totalR               DWORD [ebp - 4 * 11]
%define totalG               DWORD [ebp - 4 * 12]
%define totalB               DWORD [ebp - 4 * 13]

_motion_blur_apply_05_32:
        push    ebp                  ; saving previous rbp
        mov     ebp, esp             ; moving into current rbp rsp: base for arguments and variables
        
        sub     esp, 4 * nLocalVars  ; allocating memory on stack for local variables
        
        push    eax                  ; saving registers, just to be sure
        push    ebx
        
        ; Calculating `invNMovingLayers'.
        mov     eax, (1 << 8)
        cdq ; magic with edx, in other case idiv will spawn FPU exception.
        idiv    nMovingLayers
        mov     invNMovingLayers, eax
        
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
        ; Loading background pixel (in ecx).
        mov     eax, background
        add     eax, edx
        mov     ecx, DWORD [eax]
        
        ; Parsing background pixel R, G, B components and initializing colors accumulators.
        mov     eax, ecx
        movzx   ebx, al
        mov     backgroundPixelR, ebx
        mov     totalR, ebx
        shr     eax, 8
        movzx   ebx, al
        mov     backgroundPixelG, ebx
        mov     totalG, ebx
        shr     eax, 8
        movzx   ebx, al
        mov     backgroundPixelB, ebx
        mov     totalB, ebx
        
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
        
        movzx   eax, bl
        add     eax, totalR
        mov     totalR, eax
        shr     ebx, 8

        movzx   eax, bl
        add     eax, totalG
        mov     totalG, eax
        shr     ebx, 8
        
        movzx   eax, bl
        add     eax, totalB
        mov     totalB, eax
        
        jmp     .layer_dont_have_alpha_if_end
        
  .layer_dont_have_alpha:
        ; Adding to colors accumulators background pixel value.
        mov     eax, backgroundPixelR
        add     eax, totalR
        mov     totalR, eax

        mov     eax, backgroundPixelG
        add     eax, totalG
        mov     totalG, eax
        
        mov     eax, backgroundPixelB
        add     eax, totalB
        mov     totalB, eax
  .layer_dont_have_alpha_if_end:
        
        dec     ecx
        jnz     .next_layer
        
        ; Writing pixel average value on image.
        mov     ecx, image
        add     ecx, edx
        
        mov     ebx, invNMovingLayers
        
        mov     eax, totalR
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [ecx + 0], al

        mov     eax, totalG
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [ecx + 1], al

        mov     eax, totalB
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [ecx + 2], al
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
  
        pop     ebx                  ; restoring registers
        pop     eax
  
        add     esp, 4 * nLocalVars  ; restoring stack state
        leave                        ; mov esp,ebp / pop ebp 
        ret
