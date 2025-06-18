.386
EXTRN BIN_NUMBER: byte

PUBLIC output_highest_power_of_2

SSTK SEGMENT para STACK USE16 'STACK'
    DB 100 dup(0)
SSTK ENDS

SD_OUT3 SEGMENT para USE16 'DATA_OUT1'
    BIN_INDEX DB 15
    ; MSG_OUT DB 'Highest power of 2 (and pow of power): ', '$'
    MSG_OUT DB 'Highest power of 2: ', '$'
SD_OUT3 ENDS

SC SEGMENT para public USE16 'CODE'
	assume CS:SC, DS:SD_OUT3

; data in dl
print_char:
    push ax

    mov ax, 0
    mov ah, 02h
    int 21h

    pop ax
    ret

; data in dx for examle: mov dx, OFFSET MSG where MSG in DATA
print_str:
    push ax
    mov ah, 09h
    int 21h
    pop ax
    ret

; in - dl
; out - dl
num_to_char:
    ADD dl, 30h
    ret


; pow - ax
; out - ax
two_power:
    push cx
    push dx
    mov cx, ax
    mov ax, 0
    mov al, 1

    cmp cx, 0
    je end_power

    mov dx, 0
    mov dl, 2

    power_loop:
        mul dl
        loop power_loop


    end_power:
    pop dx
    pop cx
    ret

; out in dx
; bin_to_num:
;     ; mov bx, seg BIN_NUMBER
; 	; mov es, bx

;     mov dx, 0
;     mov cx, 16
;     cast_loop:
;         push cx
;         push dx

;         lea si, OFFSET es:BIN_NUMBER

;         mov bx, 0
;         mov bl, BIN_INDEX
;         ADD si, bx

;         mov bx, 0
;         mov bl, es:[BYTE PTR [si]]


;         mov ax, 16
;         sub ax, cx ; two power

;         call two_power

;         mul bx

;         pop dx
;         add dx, ax

;         dec BIN_INDEX
;         pop cx
;         loop cast_loop
;     ret

; out in dx
bin_to_num:
    ; mov bx, seg BIN_NUMBER
	; mov es, bx

    lea si, OFFSET es:BIN_NUMBER

    mov dx, 0
    mov cx, 15
    cast_loop:
        push cx
        ; push dx

        ; mov bx, 0
        ; mov bl, es:[BYTE PTR [si]]

        mov ax, 0
        mov al, es:[BYTE PTR [si]]

        ; pop dx
        or dx, ax
        sal dx, 1

        inc si
        pop cx
        loop cast_loop
    ret

convert_bin_number_to_word:
    mov dx, 0
    lea si, BIN_NUMBER
    mov cx, 16

convert_loop:
    shl dx, 1
    mov al, [si]
    or dl, al
    inc si
    loop convert_loop

    ret

; in - dx
; out - ax
; n & (~(n - 1))
; find_power:
;     push cx

;     cmp dx, 0
;     je zero_in
;     jne normal

;     zero_in:
;     mov ax, 1
;     jmp exit


;     normal:
;     mov ax, dx

;     mov cx, ax
;     dec cx
;     not cx

;     and ax, cx

;     exit:
;     pop cx
;     ret


find_power_num:
    push cx
    push dx

    mov bx, 0
    mov cx, ax
    test ax, ax
    jz end_find_power

check_power_loop:
    test cx, 1
    jnz not_power_of_2
    shr cx, 1
    inc bx
    cmp cx, 0
    je end_find_power
    jmp check_power_loop

not_power_of_2:
    mov dx, ax
    mov cx, 1
    mov cl, bl
    shl cx, cl
    cmp cx, 0
    je reset_result
    mov dx, ax
    div cx
    test dx, dx
    jnz reset_result
    jmp end_find_power

reset_result:
    mov bx, 0

end_find_power:
    pop dx
    pop cx
    ret

output_highest_power_of_2:
    PUSHA

    mov ax, SD_OUT3
    mov ds, ax

    mov dx, OFFSET MSG_OUT
    call print_str


    call convert_bin_number_to_word


    cmp dx, 0
    je zero_case


    bsf ax, dx
    jz zero_case


    cmp al, 10
    jl one_symbol
    mov dl, '1'
    call print_char

    sub al, 10
one_symbol:
    mov dl, al
    add dl, '0'
    call print_char
    jmp pow_found

zero_case:
    mov dl, '0'
    call print_char

pow_found:
    mov dl, 0Dh
    call print_char
    mov dl, 0Ah
    call print_char

    POPA
    ret

SC ENDS
END