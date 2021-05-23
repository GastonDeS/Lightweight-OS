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
GLOBAL endProcessSyscall
GLOBAL getPidSyscall
GLOBAL psSyscall
GLOBAL blockPidSyscall
GLOBAL unblockPidSyscall
GLOBAL mallocSyscall
GLOBAL freeSyscall
GLOBAL reallocSyscall
GLOBAL createSemSyscall
GLOBAL removeSemSyscall
GLOBAL semWaitSyscall
GLOBAL semPostSyscall
GLOBAL niceSyscall
GLOBAL yieldSyscall
GLOBAL printSemSyscall
GLOBAL printMemSyscall
GLOBAL checkMemorySyscall
GLOBAL pipeSyscall
GLOBAL pipeCloseSyscall
GLOBAL pipeWriteSyscall
GLOBAL pipeReadSyscall


section .text
pipeReadSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 35
  int 80h

  mov rsp, rbp
  pop rbp
  ret
pipeWriteSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 34
  int 80h

  mov rsp, rbp
  pop rbp
  ret
pipeCloseSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 33
  int 80h

  mov rsp, rbp
  pop rbp
  ret
pipeSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 32
  int 80h

  mov rsp, rbp
  pop rbp
  ret
checkMemorySyscall:
  push rbp
  mov rbp, rsp

  mov rax, 31
  int 80h

  mov rsp, rbp
  pop rbp
  ret

printMemSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 30
  int 80h

  mov rsp, rbp
  pop rbp
  ret
printSemSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 29
  int 80h

  mov rsp, rbp
  pop rbp
  ret
yieldSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 28
  int 80h

  mov rsp, rbp
  pop rbp
  ret
niceSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 27
  int 80h

  mov rsp, rbp
  pop rbp
  ret

semPostSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 26
  int 80h

  mov rsp, rbp
  pop rbp
  ret
semWaitSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 25
  int 80h

  mov rsp, rbp
  pop rbp
  ret

removeSemSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 24
  int 80h

  mov rsp, rbp
  pop rbp
  ret

createSemSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 23
  int 80h

  mov rsp, rbp
  pop rbp
  ret

reallocSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 22
  int 80h

  mov rsp, rbp
  pop rbp
  ret

freeSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 21
  int 80h

  mov rsp, rbp
  pop rbp
  ret
mallocSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 20
  int 80h

  mov rsp, rbp
  pop rbp
  ret

unblockPidSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 19
  int 80h

  mov rsp, rbp
  pop rbp
  ret

blockPidSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 18
  int 80h

  mov rsp, rbp
  pop rbp
  ret

psSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 17
  int 80h

  mov rsp, rbp
  pop rbp
  ret

getPidSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 16 
  int 80h

  mov rsp, rbp
  pop rbp
  ret

endProcessSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 15 
  int 80h

  mov rsp, rbp
  pop rbp
  ret

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
