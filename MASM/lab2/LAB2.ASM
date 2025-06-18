.MODEL TINY
.DOSSEG
.DATA
    MSG DB "Hello, World!", 0Dh, 0Ah, '$'
.CODE
.STARTUP
    MOV DX, OFFSET MSG
    MOV CX, 3
    print:
        mov AH, 09h
        int 21h
        loop print

    MOV AH, 4Ch
    INT 21h
END