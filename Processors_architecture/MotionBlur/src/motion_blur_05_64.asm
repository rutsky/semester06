; motion_blur_05_64.asm
; Motion blur effect.
; Implementation 4 rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 64-bit Linux version.
;

global _motion_blur_apply_05_64

; void _motion_blur_apply_05_64(
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

_motion_blur_apply_05_64:
        push    rbp                  ; saving previous rbp
        mov     rbp, rsp             ; moving into current rbp rsp: base for arguments and variables
        
        sub     rsp, 8 * nLocalVars  ; allocating memory on stack for local variables
        
        push    rax                  ; saving registers, just to be sure
        push    rbx
        
        ; Reorganizing input variables.
        mov     h, arg_h
        mov     scanlineLen, arg_scanlineLen
        
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
        ; Loading background pixel (in ecx).
        mov     rax, background
        add     rax, rdx
        mov     ecx, DWORD [rax]
        
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
        shl     eax, 3     ; 3 == log(2, sizeof(byte_type *))
        add     rax, movingLayers
        mov     rax, [rax] ; retrieved layer start address
        add     rax, rdx
        mov     ebx, DWORD [rax]
        
        ; Checking is pixel value has alpha.
        cmp     ebx, 0x00ffffff
        jbe     .layer_dont_have_alpha
        
        ; Adding layer pixel value to accumulators.
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
        mov     rcx, image
        add     rcx, rdx
        
        mov     ebx, invNMovingLayers
        
        mov     eax, totalR
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [rcx + 0], al

        mov     eax, totalG
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [rcx + 1], al

        mov     eax, totalB
        imul    eax, ebx
        shr     eax, 8
        mov     BYTE [rcx + 2], al
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
  
        pop     rbx                  ; restoring registers
        pop     rax
  
        add     rsp, 8 * nLocalVars  ; restoring stack state
        leave                        ; mov esp,ebp / pop ebp 
        ret
