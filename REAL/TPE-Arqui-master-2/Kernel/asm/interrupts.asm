GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq80Handler

EXTERN irqDispatcher

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1

	push $
	push rsp
	mov [rsp+8], rsp

	pushState


  mov rsi, rsp ;puntero a los registros

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState

	;saco los dos push de antes de hacer un pushState
	mov [regBackUp], rax
	pop rax
	pop rax
	mov rax, [regBackUp]

	iretq
%endmacro



; %macro exceptionHandler 1
; 	pushState
;
; 	mov rdi, %1 ; pasaje de parametro
; 	call exceptionDispatcher
;
; 	popState
; 	iretq
; %endmacro

_cli:
	cli
	ret


_sti:
	sti
	ret

_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

_irq80Handler:
	irqHandlerMaster 80

picMasterMask:
	 push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
		push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


section .bss
regBackUp resb 8
