GLOBAL drawStringSysCall
GLOBAL readKeyboardSysCall
GLOBAL isKeyboardEmptySyscall
GLOBAL clearDisplaySyscall
GLOBAL drawRectSyscall
GLOBAL getTimeSyscall
GLOBAL getRegistersSyscall
GLOBAL getMemSyscall
GLOBAL drawMatrixSyscall
GLOBAL readErrorSyscall
GLOBAL setKeyPressedFunctionSyscall
GLOBAL isMayusSyscall
GLOBAL getTicksSyscall
GLOBAL sbrkSyscall
GLOBAL createProcessSyscall
section .text


createProcessSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 14 
  int 80h

  mov rsp, rbp
  pop rbp
  ret

sbrkSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 13 ; la 12 ya estaba ocupada
  int 80h

  mov rsp, rbp
  pop rbp
  ret

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

drawMatrixSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 6 ; drawMatrix syscall
  int 80h

  mov rsp, rbp
  pop rbp
  ret

getTimeSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 8
  int 80h

  mov rsp, rbp
  pop rbp
  ret

getRegistersSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 9
  int 80h

  mov rsp, rbp
  pop rbp
  ret

getMemSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 10 ;ID get43BytesFromAddress
  int 80h

  mov rsp, rbp
  pop rbp
  ret

readErrorSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 12 ;ID readError
  int 80h

  mov rsp, rbp
  pop rbp
  ret

getTicksSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 7 ;ID get43BytesFromAddress
  int 80h

  mov rsp, rbp
  pop rbp
  ret
