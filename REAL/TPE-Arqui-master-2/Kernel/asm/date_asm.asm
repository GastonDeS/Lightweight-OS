GLOBAL getTimeInfo

section .text

getTimeInfo:
  push rbp
  mov rbp, rsp

  push rbx
  push rcx

  mov rcx, 10
  mov rax, rdi
  out 70h, al
  in al, 71h
  mov bx, ax
  and ax, 0xF0
  and bx, 0x0F
  shr rax, 4
  mul rcx
  add ax, bx

  pop rcx
  pop rbx

  mov rsp, rbp
  pop rbp
  ret
