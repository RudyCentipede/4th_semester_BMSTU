.386

EXTRN input_bin_number: near
EXTRN output_unsigned_hex: near
EXTRN output_uint_to_char: near
EXTRN output_highest_power_of_2: near

EXTRN BIN_NUMBER:byte


SSTK SEGMENT para STACK USE16 'STACK'
    DB 100 dup(0)
SSTK ENDS

SD SEGMENT para USE16 'DATA'
    MENU_FUNCS DW 4 dup(?)
    MSG_MENU  DB 0ah, 0dh,
        '1) Input bin number', 0ah, 0dh,
        '2) Output unsigned hex', 0ah, 0dh,
        '3) Output uint to char', 0ah, 0dh,
        '4) Output highest power of 2', 0ah, 0dh,
        '0) EXIT', '$'
    MSG_INPUT_ITEM DB 'Enter menu item (0-4): ', '$'
SD ENDS


SC SEGMENT para public USE16 'CODE'
	assume CS:SC, DS:SD

print_char:
    push ax
    mov ah, 02h
    int 21h
    pop ax
    ret

go_to_new_str:
    push dx
    mov dl, 10
    call print_char

    mov dl, 13
    call print_char
    pop dx
    ret

fill_array:
    lea si, OFFSET MENU_FUNCS

    mov WORD PTR [si], input_bin_number
    add si, 2

    mov WORD PTR [si], output_unsigned_hex
    add si, 2

    mov WORD PTR [si], output_uint_to_char
    add si, 2

    mov WORD PTR [si], output_highest_power_of_2
    
    ret

print_str:
    push ax
    mov ah, 09h
    int 21h
    pop ax
    ret

print_menu:
    mov dx, OFFSET MSG_MENU

    call print_str

    call go_to_new_str

    ret

; data in al
scan_char_with_echo:
    mov ah, 01h
	int 21h	
	ret

; in - al
; out - al
char_to_num:
    SUB al, 30h
    ret

scan_menu_item:
    mov ax, 0
    mov dx, OFFSET MSG_INPUT_ITEM
    call print_str

    call scan_char_with_echo
    call char_to_num

    ret

call_func:
    push ax

    dec al

    mov bx, 2
    mul bl

    lea bx, MENU_FUNCS
    add bx, ax

    call WORD PTR [bx]

    pop ax
    ret

main:
    mov ax, SD
    mov ds, ax

    call fill_array

    menu_loop:
        mov ax, SD
        mov ds, ax

        call print_menu
        call scan_menu_item

        cmp al, 0
        je exit

        call go_to_new_str

        call call_func
        
        call go_to_new_str
        jmp menu_loop
    

    exit:
    mov ax, 4c00h
	int 21h

SC ENDS
END main