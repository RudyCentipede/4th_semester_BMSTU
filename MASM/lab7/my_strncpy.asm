; Принимает три параметра:
; rdi - указатель на dst
; rsi - указатель на src
; rdx - длину src
global my_strncpy


section .text
my_strncpy:
    push rdi

    mov rcx, rdx
    ; проверим разные случаи расположения указателей
    cmp rsi, rdi
    ; ==
    je exit
    ; >
    jg copy
    ; <
    mov rax, rdi
    sub rax, rsi ; количество символов между dst и src
    cmp rax, rcx ; проверяем достаточно ли места для копирования
    jge copy

    ; перекрываются
    add rdi, rcx
    add rsi, rcx
    inc rcx
    std


    copy:
    rep movsb
    cld

    exit:
    pop rdi
    add rdi, rdx
    mov al, 0
    stosb

    ret