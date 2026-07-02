; ============================================================================
; PongOS Boot Entry
; GRUB2 + Multiboot2
; Starts in 32-bit Protected Mode
; ============================================================================

BITS 32

global start
extern kernel_main

; ============================================================================
; Multiboot2 Header
; ============================================================================

section .multiboot
align 8

MB2_MAGIC      equ 0xE85250D6
MB2_ARCH       equ 0
MB2_LENGTH     equ header_end - header_start
MB2_CHECKSUM   equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

header_start:

dd MB2_MAGIC
dd MB2_ARCH
dd MB2_LENGTH
dd MB2_CHECKSUM

; ------------------------------------------------------------------------
; Request Framebuffer (800x600x32)
; ------------------------------------------------------------------------

align 8

dw 5
dw 0
dd 20

dd 800
dd 600
dd 32

; ------------------------------------------------------------------------
; End Tag
; ------------------------------------------------------------------------

align 8

dw 0
dw 0
dd 8

header_end:

; ============================================================================
; Stack
; ============================================================================

section .bss
align 16

stack_bottom:
    resb 16384

stack_top:

; ============================================================================
; Entry Point
; ============================================================================

section .text

start:

    cli

    mov esp, stack_top

    ; Stack 16-byte aligned
    and esp, 0xFFFFFFF0

    ; Initialize x87 FPU state for floating-point support
    fninit

    ; GRUB Multiboot2
    ; eax = magic
    ; ebx = multiboot information pointer

    push ebx
    push eax

    call kernel_main

.hang:

    cli

.loop:

    hlt
    jmp .loop