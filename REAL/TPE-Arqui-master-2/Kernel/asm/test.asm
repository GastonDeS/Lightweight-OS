GLOBAL testeo
GLOBAL testeo2

section .text


testeo2:

testeo:
  push rbp
  mov rbp, rsp

  ;seteo los argumentos
  mov rdx, 50
  mov rbx, 0
  mov rcx, 0
  mov r8, 0FFFFFFx
  mov r9, 15
  mov r10, 2

  ;me fijo cual string toca
  mov rax, [strNum]
  cmp rax, 0
  je .setStr1
  cmp rax, 1
  je .setStr2
  cmp rax, 2
  je .setStr3
  cmp rax, 3
  je .setStr4
  mov rax, 0
  mov [strNum], rax
  jmp .setStr1

  .setStr1:
  mov rsi, str1
  jmp .next

  .setStr2:
  mov rsi, str2
  jmp .next

  .setStr3:
  mov rsi, str3
  jmp .next

  .setStr4:
  mov rsi, str4
  jmp .next

  ;ID syscall writeSTR
  .next:
  mov rax, 1

  int 80h

  mov rax, [strNum]
  inc rax
  mov [strNum], rax

  mov rsp, rbp
  pop rbp

  ret

section .bss
string resb 5

section .data
strNum db 0

section .rodata

str1 db "Este es el string numero 1", 0
str2 db "Camaron que se duerme se lo lleva la corriente", 0
str3 db "Mas vale pajaro en mano que 100 volando xD :v", 0
str4 db "a", 0
