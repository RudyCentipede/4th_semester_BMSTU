StkSeg SEGMENT PARA STACK 'STACK'
    DB 200h DUP (?)
StkSeg ENDS

Data1 SEGMENT WORD 'DATA'
    BufSize DB 7
    BufLen DB ?
    InputBuffer DB 7 DUP(?)
Data1 ENDS

Data2 SEGMENT WORD 'DATA'
    SumResult DB ?
Data2 ENDS

Code SEGMENT WORD 'CODE'
    ASSUME CS:Code, DS:Data1, ES:Data2

Start:
    ; Ввод
    MOV AX, Data1
    MOV DS, AX

    MOV DX, OFFSET BufSize
    MOV AH, 0Ah
    INT 21h

    MOV AX, Data2
    MOV ES, AX

    ; Извлекаем символы
    MOV AL, InputBuffer+1
    SUB AL, '0'
    MOV BL, InputBuffer+4
    SUB BL, '0'

    ; Сложение
    ADD AL, BL

    ADD AL, '0'
    MOV ES:SumResult, AL

    ; Вывод
    MOV DL, 13
    INT 21h
    MOV DL, ES:SumResult
    MOV AH, 2
    INT 21h

    MOV AH, 4Ch
    INT 21h


Code ENDS
END Start
