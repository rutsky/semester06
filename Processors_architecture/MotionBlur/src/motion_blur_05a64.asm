; motion_blur_05a64.asm
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
;        byte_type const *const *movingLayers // [rbp + 8 * 2]
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

; Local variables.
%define invNMovingLayers     DWORD [rbp - 8 *  1]
%define y                    DWORD [rbp - 8 *  2]
%define x                    DWORD [rbp - 8 *  3]
%define idx                  DWORD [rbp - 8 *  4]
%define lastLayerMovingPixel DWORD [rbp - 8 *  5]
%define backgroundPixel      DWORD [rbp - 8 *  6]
%define totalR               DWORD [rbp - 8 *  7]
%define totalG               DWORD [rbp - 8 *  8]
%define totalB               DWORD [rbp - 8 *  9]
%define r                    DWORD [rbp - 8 * 10]
%define g                    DWORD [rbp - 8 * 11]
%define b                    DWORD [rbp - 8 * 12]

%define h                    DWORD [rbp - 8 * 13]
%define scanlineLen          DWORD [rbp - 8 * 14]

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
        
        ;mov ebx, ecx
        
        ; Checking is pixel value has alpha (in ARGB model it means that pixel value > 0xffffff).
        cmp     ecx, 0x00ffffff
        jbe     .last_layer_not_transparent
        
  ;last_layer_is_transparent:
        ; Writing last layer pixel on image.
        mov     rax, image
        add     rax, rdx
        mov     DWORD [rax], ecx
        
        jmp     .last_layer_not_transparent_if_end
        
  .last_layer_not_transparent:
        ; Loading background pixel (in ecx).
        mov     rax, background
        add     rax, rdx
        mov     ecx, DWORD [rax]
        
        ; Writing background pixel on image.
        mov     rax, image
        add     rax, rdx
        mov     DWORD [rax], ecx
  .last_layer_not_transparent_if_end:
  
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
