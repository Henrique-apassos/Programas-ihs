section .data
divisor: dd 3.0 ; Constante para entrar na fpu

section .text


global calc_circ_area
global calc_cone_vol

calc_circ_area:

;; criar novo stack frame
push ebp
mov ebp, esp

;; criar espaço para variaveis locais
;; se necessario, esp - (4 * qnt de local var)

;; inicia a FPU
finit

;; calula area
fldpi               ;; Pi em st0
fld dword[ebp + 8]  ;; R em st0 e Pi em st1
fmul st0, st0       ;; R^2 em st0 e Pi em st1
fmulp st1, st0      ;; Pi*R^2 em st0

;; destroi stack frame
mov esp, ebp
pop ebp

;; retorna. retorno em st0
ret


calc_cone_vol:

enter 0, 0 ; Criando novo stack frame

finit ; inicia FPU

;; Calcular volume
fld dword[ebp + 8] ; Area do circulo em st0
fld dword[ebp + 12] ; Altura no topo da pilha (st0) e Area em st1
fmul st1 ; Area * altura em st0
fdiv dword[divisor] ; (A * h)/ 3 em st0


leave ; Retorna os valores antigos da stack
ret ; Retorna a funcao