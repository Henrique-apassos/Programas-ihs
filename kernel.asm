; comentarios

ORG 0x7C00
BITS 16
    jmp start

msg: db "Eu sou seu pai", 0x0D, 0x0A, 0

start:

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov si, msg
    call contVogal
    mov si, msg ; Redireciona si para o comeco da string
    call print_string

    xor ax, ax
    mov ax, cx

    call print_number

end:
    jmp $ ; halt


print_string:
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    ret

print_number:
    mov bx, 10
    mov cx, 0
.loop1:
    mov dx, 0
    div bx
    ; resposta vai ta no ax, resto no dx
    add dx, 48
    push dx
    inc cx
    cmp ax, 0
    jne .loop1
.loop2:
    pop ax
    mov ah, 0x0E
    int 0x10
    loop .loop2
.done:
    ret

contVogal:
    mov cx, 0
.prox_char:
    lodsb
    or al, al
    jz .done
    ; Verifica se eh vogal
    cmp al, 0x41 ; A
    je .increm
    cmp al, 0x61 ; a
    je .increm
    cmp al, 0x45 ; E
    je .increm
    cmp al, 0x65 ; e
    je .increm
    cmp al, 0x49 ; I
    je .increm
    cmp al, 0x69 ; i
    je .increm
    cmp al, 0x4F ; O
    je .increm
    cmp al, 0x6F ; o
    je .increm
    cmp al, 0x55 ; U
    je .increm
    cmp al, 0x75 ; u
    je .increm
    ; Senao eh vogal, passa para o proximo caractere
    jmp .prox_char

.increm: ; Funcao para incrementar o contador cx
    inc cx
    jmp .prox_char

.done: ; Faz o retorno da funcao
    ret


; assinatura de boot

    times 510-($-$$) db 0
    dw 0xAA55
