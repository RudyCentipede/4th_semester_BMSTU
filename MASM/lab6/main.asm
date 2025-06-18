.MODEL tiny

.data
org 100h

.code
.startup

jmp change_autorepeat_handler

speed db 1fh ; начальная скорость
counter db 0

jmp_old_handler:
    db 0EAh
old_autorepeat_handler dw 0,0


new_autorepeat_handler:
inc counter

cmp counter, 18
jge change_speed_autorepeat_handler ; counter >= 18
jmp jmp_old_handler

change_speed_autorepeat_handler:
cmp speed, 0 ; если speed <= 0, сбросить на максимальную скорость
jle reset_speed
mov al, 0f3h
out 60h, al
mov al, speed
out 60h, al
dec speed ; уменьшить скорость
mov counter, 0
jmp jmp_old_handler

reset_speed:
mov speed, 1fh ; сбросить скорость на максимальную (самую медленную)
mov al, 0f3h
out 60h, al
mov al, speed
out 60h, al
mov counter, 0
jmp jmp_old_handler


change_autorepeat_handler:
mov ax, 3508h
int 21h
mov old_autorepeat_handler, bx
mov old_autorepeat_handler + 2, es
mov ax, 2508h ; ah: 25h - функция устанавливает новый обработчик прерывания
lea dx, new_autorepeat_handler ; адрес new_autorepeat_handler кладется в dx
int 21h

lea dx, change_autorepeat_handler ; адрес change_autorepeat_handler в dx
int 27h
end