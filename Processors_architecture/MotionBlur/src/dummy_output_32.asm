; dummy_output_32.asm
; Motion blur effect.
; Dummy output rewritten in assembler.
; Vladimir Rutsky <altsysrq@gmail.com>
; 09.06.2009

;
; 32-bit version.
;

[bits 32]
[section .bss align=16]
[section .data align=16]
[section .text align=16]
[global _motion_blur_apply_dummy_32]

; void _motion_blur_apply_dummy_32(
;        byte_type *image,                    // [ebp + 4 * 2]
;        int w,                               // [ebp + 4 * 3]
;        int h,                               // [ebp + 4 * 4]
;        int scanlineLen,                     // [ebp + 4 * 5]
;        byte_type const *background,         // [ebp + 4 * 6]
;        int nMovingLayers,                   // [ebp + 4 * 7]
;        byte_type const *const *movingLayers // [ebp + 4 * 8]
;      );

%define nLocalVars    20

; Arguments.
%define image           DWORD [ebp + 4 *  1]
%define w               DWORD [ebp + 4 *  2]
%define h               DWORD [ebp + 4 *  3]
%define scanlineLen     DWORD [ebp + 4 *  4]
%define background      DWORD [ebp + 4 *  5]
%define nMovingLayers   DWORD [ebp + 4 *  6]
%define movingLayers    DWORD [ebp + 4 *  7]

; Local variables.
%define y               DWORD [ebp - 4 *  8]
%define x               DWORD [ebp - 4 *  9]
%define idx             DWORD [ebp - 4 * 10]

_motion_blur_apply_dummy_32:
        push    ebp                  ; saving previous rbp
        mov     ebp, esp             ; moving into current rbp rsp: base for arguments and variables
        
        sub     esp, 4 * nLocalVars  ; allocating memory on stack for local variables
        
        push    eax                  ; saving registers, just to be sure
        push    ebx
        
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
        
        ; Calculating index (in edx).
        mov     eax, y
        imul    eax, scanlineLen
        mov     edx, x
        shl     edx, 2
        add     edx, eax
        
        ; Loading background pixel (in ecx).
        mov     eax, background
        add     eax, edx
        mov     ecx, DWORD [eax]
        
        ; Writing background pixel on image.
        mov     eax, image
        add     eax, edx
        mov     DWORD [eax], ecx
  
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
