; Принимает три параметра через стек:
; [esp+4] - указатель на dst
; [esp+8] - указатель на src
; [esp+12] - длину src
global my_strncpy

section .text
my_strncpy:
    push ebp
    mov ebp, esp
    push edi
    push esi
    push ebx

    mov edi, [ebp+8]
    mov esi, [ebp+12]
    mov edx, [ebp+16]
    mov ecx, edx

    cmp esi, edi
    je exit
    jg copy

    mov eax, edi
    sub eax, esi
    cmp eax, ecx
    jge copy

    add edi, ecx
    add esi, ecx
    inc ecx
    std

copy:
    rep movsb
    cld

exit:
    mov edi, [ebp+8]
    add edi, edx
    mov al, 0
    stosb

    pop ebx
    pop esi
    pop edi
    mov esp, ebp
    pop ebp
    ret