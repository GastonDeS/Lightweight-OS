GLOBAL forceDivZero
GLOBAL forceInvalidOPCode
section .text

forceDivZero:
push rbp
mov rbp, rsp

xor rcx, rcx
xor rax, rax
div rcx

mov rsp, rbp
pop rbp
ret

forceInvalidOPCode:
push rbp
mov rbp, rsp

ud2

mov rsp, rbp
pop rbp
ret
