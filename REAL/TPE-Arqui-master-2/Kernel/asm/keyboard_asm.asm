GLOBAL keyActivated
GLOBAL getKeyCode

section .text

;---------------------------------------------------------------------------
;Function: keyActivated
;Returns in rax 1 if a key is being pressed, 0 if not
;---------------------------------------------------------------------------
keyActivated:
  push rbp
  mov rbp, rsp

  xor rax, rax
  in al, 64h      ;lee el puerto 64
  and ax, 0x01    ;dejo solo el ultimo bit prendido
              ;Si es 0, no hay tecla prendida
              ;Si es 1, hay tecla apretada
  mov rsp, rbp
  pop rbp
  ret

;---------------------------------------------------------------------------
;Function: getKeyCode
;Returns in rax the code of the key readed from the port 60h
;---------------------------------------------------------------------------

getKeyCode:
  push rbp
  mov rbp, rsp

  xor rax, rax
  in al, 60h
  
  mov rsp, rbp
  pop rbp
  ret
