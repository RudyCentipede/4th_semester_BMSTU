bits 64
global main

%define GTK_WIN_POS_CENTER 1
%define GTK_WIN_WIDTH 300
%define GTK_WIN_HEIGHT 200

extern exit
extern gtk_init
extern gtk_main
extern gtk_main_quit
extern gtk_window_new
extern gtk_widget_show_all
extern gtk_window_set_title
extern gtk_window_set_default_size
extern gtk_box_new
extern gtk_container_add
extern gtk_entry_new
extern gtk_button_new_with_label
extern gtk_label_new
extern g_signal_connect_data
extern gtk_entry_get_text
extern gtk_label_set_text
extern gtk_box_pack_start

section .bss
    window:     resq 1
    box:        resq 1
    entry_num:  resq 1
    entry_exp:  resq 1
    btn:        resq 1
    label:      resq 1

section .rodata
    signal.destroy: db "destroy", 0
    signal.clicked: db "clicked", 0
    title:          db "Калькулятор степеней", 0
    btn_text:       db "Вычислить", 0
    err_msg:        db "Ошибка ввода", 0
    fmt_str:        db "%d", 0

section .text

__destroy_window:
    xor rdi, rdi
    call gtk_main_quit
    ret

__calculate:
    push rbp
    mov rbp, rsp
    sub rsp, 64

    ; Получаем число
    mov rdi, [rel entry_num]
    call gtk_entry_get_text
    test rax, rax
    jz .error
    mov rdi, rax
    call atoi
    mov [rsp+32], eax  ; Сохраняем число

    ; Получаем степень
    mov rdi, [rel entry_exp]
    call gtk_entry_get_text
    test rax, rax
    jz .error
    mov rdi, rax
    call atoi
    mov ecx, eax       ; Сохраняем степень

    ; Проверка на отрицательную степень
    cmp ecx, 0
    jl .error

    ; Вычисление степени (2^1)
    mov eax, 1
    test ecx, ecx
    jz .set_result
    mov ebx, [rsp+32]
    .loop:
        imul eax, ebx
        jo .error      ; Проверка переполнения
        dec ecx
        jnz .loop

.set_result:
    ; Преобразуем результат в строку
    lea rdi, [rsp+48] ; Буфер для строки
    mov esi, fmt_str
    mov edx, eax
    call sprintf
    test eax, eax
    js .error

    ; Устанавливаем результат
    mov rdi, [rel label]
    lea rsi, [rsp+48]
    call gtk_label_set_text
    jmp .end

.error:
    mov rdi, [rel label]
    mov rsi, err_msg
    call gtk_label_set_text

.end:
    add rsp, 64
    leave
    ret

; Внешняя функция sprintf для форматирования
extern sprintf

; Упрощенная atoi с проверками
atoi:
    xor eax, eax
    xor ecx, ecx
    movzx r8d, byte [rdi]
    cmp r8b, '-'
    sete cl
    add rdi, rcx
    .loop:
        movzx edx, byte [rdi]
        test dl, dl
        jz .done
        sub edx, '0'
        js .error
        cmp edx, 9
        ja .error
        imul eax, 10
        jo .error
        add eax, edx
        jo .error
        inc rdi
        jmp .loop
    .error:
    xor eax, eax
    .done:
    test ecx, ecx
    jz .positive
    neg eax
    .positive:
    ret

main:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    ; Инициализация GTK
    xor rdi, rdi
    xor rsi, rsi
    call gtk_init

    ; Создание окна
    xor rdi, rdi
    call gtk_window_new
    mov [rel window], rax

    ; Настройка окна
    mov rdi, [rel window]
    mov rsi, title
    call gtk_window_set_title

    mov rdi, [rel window]
    mov rsi, GTK_WIN_WIDTH
    mov rdx, GTK_WIN_HEIGHT
    call gtk_window_set_default_size

    ; Создание контейнера
    mov rdi, 1       ; GTK_ORIENTATION_VERTICAL
    mov rsi, 5       ; spacing
    call gtk_box_new
    mov [rel box], rax

    ; Создание элементов
    call gtk_entry_new
    mov [rel entry_num], rax

    call gtk_entry_new
    mov [rel entry_exp], rax

    mov rdi, btn_text
    call gtk_button_new_with_label
    mov [rel btn], rax

    mov rdi, 0
    call gtk_label_new
    mov [rel label], rax

    ; Упаковка элементов
    mov rdi, [rel box]
    mov rsi, [rel entry_num]
    mov rdx, 1       ; expand
    mov rcx, 1       ; fill
    mov r8, 0        ; padding
    call gtk_box_pack_start

    mov rdi, [rel box]
    mov rsi, [rel entry_exp]
    mov rdx, 1
    mov rcx, 1
    mov r8, 0
    call gtk_box_pack_start

    mov rdi, [rel box]
    mov rsi, [rel btn]
    mov rdx, 0
    mov rcx, 0
    mov r8, 0
    call gtk_box_pack_start

    mov rdi, [rel box]
    mov rsi, [rel label]
    mov rdx, 1
    mov rcx, 1
    mov r8, 0
        call gtk_box_pack_start

        ; Добавление контейнера
        mov rdi, [rel window]
        mov rsi, [rel box]
        call gtk_container_add

        ; Подключение сигналов
        mov rdi, [rel btn]
        mov rsi, signal.clicked
        mov rdx, __calculate
        xor rcx, rcx
        xor r8, r8
        xor r9, r9
        call g_signal_connect_data

        mov rdi, [rel window]
        mov rsi, signal.destroy
        mov rdx, __destroy_window
        xor rcx, rcx
        xor r8, r8
        xor r9, r9
        call g_signal_connect_data

        ; Отображение
        mov rdi, [rel window]
        call gtk_widget_show_all

        ; Главный цикл
        call gtk_main

        add rsp, 32
        leave
        ret