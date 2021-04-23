GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq80Handler
GLOBAL _exc00Handler
GLOBAL _exc06Handler
GLOBAL saveInitialConditions


EXTERN irqDispatcher
EXTERN exceptionDispatcher

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
	pushState

  mov rsi, rsp ;puntero a los registros

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState

	;saco los dos push de antes de hacer un pushState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp; le pasamos los registros
	call exceptionDispatcher

	mov rax, [initConditions]; El rsp
	mov [rsp+18*8], rax
	mov rax, [initConditions+1*8]; El rip
	mov [rsp+15*8], rax

	popState
	iretq
%endmacro

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

_exc00Handler:
	exceptionHandler 0

_exc06Handler:
	exceptionHandler 6

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

saveInitialConditions: ;Recibe por rdi el address del sampleCodeModule
	mov rax, rsp
	mov [initConditions], rax
	mov rax, rdi
	mov [initConditions + 1*8], rax
	ret

section .bss
initConditions resb 16; Primeros 8 bits, el rsp. Segundos 8 bits, el rip
