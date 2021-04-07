GLOBAL drawStringSysCall
GLOBAL readKeyboardSysCall
GLOBAL isKeyboardEmptySyscall
GLOBAL clearDisplaySyscall
GLOBAL drawRectSyscall

section .text


;TODO describir esta syscall
drawStringSysCall:
  push rbp
  mov rbp, rsp

  mov rax, 1 ;ID write str
  mov r10, [rbp+2*8] ;sexto argumento
  mov r11, [rbp+3*8] ;septimo argumento

  int 80h

  mov rsp, rbp
  pop rbp
  ret

readKeyboardSysCall:
  push rbp
  mov rbp, rsp

  mov rax, 0    ;ID read
  int 80h

  mov rsp, rbp
  pop rbp
  ret

isKeyboardEmptySyscall:
  push rbp
  mov rbp, rsp

  mov rax, 11    ;ID bufferEmpty
  int 80h

  mov rsp, rbp
  pop rbp
  ret

clearDisplaySyscall:
  push rbp
  mov rbp, rsp

  mov rax, 2    ;ID clear Display
  int 80h

  mov rsp, rbp
  pop rbp
  ret

drawRectSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 5 ; drawRectangle syscall
  int 80h

  mov rsp, rbp
  pop rbp
  ret