.MODEL SMALL
.STACK 100H

.DATA
    rows        DB ?
    cols        DB ?
    matrix      DB 81 DUP(0)

    msg_rows    DB 'Enter rows (1-9): $'
    msg_cols    DB 'Enter columns (1-9): $'
    msg_matrix  DB 13,10,'Enter matrix (digits 0-9):',13,10,'$'
    msg_result  DB 13,10,'Result matrix:',13,10,'$'
    newline     DB 13,10,'$'
    space       DB ' $'

.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX

    CALL SCAN_DIMENSIONS
    CALL SCAN_MATRIX
    CALL ODD_TO_ZEROS
    CALL PRINT_MATRIX

    MOV AH, 4CH
    INT 21H
MAIN ENDP

; ==============================================
; Ввод размеров матрицы
; ==============================================
SCAN_DIMENSIONS PROC
    MOV AH, 09H
    LEA DX, msg_rows
    INT 21H

    MOV AH, 01H
    INT 21H
    SUB AL, '0'
    MOV rows, AL

    CALL PRINT_NEWLINE

    MOV AH, 09H
    LEA DX, msg_cols
    INT 21H

    MOV AH, 01H
    INT 21H
    SUB AL, '0'
    MOV cols, AL

    CALL PRINT_NEWLINE
    RET
SCAN_DIMENSIONS ENDP

; ==============================================
; Ввод элементов матрицы
; ==============================================
SCAN_MATRIX PROC
    MOV AH, 09H
    LEA DX, msg_matrix
    INT 21H

    XOR BX, BX          ; BX = номер текущей строки (0..8)
    MOV CH, 0
    MOV CL, rows        ; CX = количество строк

INPUT_ROWS:
    PUSH CX
    XOR SI, SI         ; номер текущего столбца (0..8)
    MOV CL, cols       ; количество столбцов

    ; Вычисляем смещение для текущей строки (BX * 9)
    MOV DI, BX
    MOV AX, DI
    MOV DX, 9
    MUL DX
    MOV DI, AX         ; DI = смещение строки

INPUT_COLS:
    ; Ввод цифры
    MOV AH, 01H
    INT 21H
    SUB AL, '0'
    MOV matrix[DI], AL

    ; Вывод пробела
    MOV AH, 09H
    LEA DX, space
    INT 21H

    INC DI
    INC SI
    LOOP INPUT_COLS

    CALL PRINT_NEWLINE
    POP CX
    INC BX
    LOOP INPUT_ROWS
    RET
SCAN_MATRIX ENDP

; ==============================================
; Замена нечетных на 0
; ==============================================
ODD_TO_ZEROS PROC
    XOR BX, BX          ; BX = номер текущей строки
    MOV CH, 0
    MOV CL, rows        ; CX = количество строк

PROCESS_ROWS:
    PUSH CX
    XOR SI, SI         ; номер текущего столбца
    MOV CL, cols       ; количество столбцов

    ; Вычисляем смещение для текущей строки (BX * 9)
    MOV DI, BX
    MOV AX, DI
    MOV DX, 9
    MUL DX
    MOV DI, AX         ; DI = смещение строки

PROCESS_COLS:
    ; Проверка на нечетность
    MOV AL, matrix[DI]
    TEST AL, 1
    JZ EVEN_NUM

    MOV matrix[DI], 0

EVEN_NUM:
    INC DI
    INC SI
    LOOP PROCESS_COLS

    POP CX
    INC BX
    LOOP PROCESS_ROWS
    RET
ODD_TO_ZEROS ENDP

; ==============================================
; Вывод результата
; ==============================================
PRINT_MATRIX PROC
    MOV AH, 09H
    LEA DX, msg_result
    INT 21H

    XOR BX, BX
    MOV CH, 0
    MOV CL, rows

OUTPUT_ROWS:
    PUSH CX
    XOR SI, SI
    MOV CL, cols

    ; Вычисляем смещение для текущей строки (BX * 9)
    MOV DI, BX
    MOV AX, DI
    MOV DX, 9
    MUL DX
    MOV DI, AX        ; DI = смещение строки

OUTPUT_COLS:
    MOV DL, matrix[DI]
    ADD DL, '0'
    MOV AH, 02H
    INT 21H

    ; Вывод пробела
    MOV AH, 09H
    LEA DX, space
    INT 21H

    INC DI
    INC SI
    LOOP OUTPUT_COLS

    CALL PRINT_NEWLINE
    POP CX
    INC BX
    LOOP OUTPUT_ROWS
    RET
PRINT_MATRIX ENDP

; ==============================================
; Вывод новой строки
; ==============================================
PRINT_NEWLINE PROC
    MOV AH, 09H
    LEA DX, newline
    INT 21H
    RET
PRINT_NEWLINE ENDP

END MAIN