GLOBAL _xadd    
section .text
_xadd:
    mov rax, rdi
    lock xadd [rsi],eax 
    ret
