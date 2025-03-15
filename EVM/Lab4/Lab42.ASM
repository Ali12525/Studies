stseg   segment para stack
        dw 16 dup(?)
stseg   ends

dseg    segment para
        x       dw 1, 0, 1, 2, 3, 0, 4, 5
        y       dw ?
dseg    ends

cseg    segment para

main proc far
        assume cs:cseg, ds:dseg, ss:stseg
        push ds
	mov ax, 0
	push ax
        mov     ax, dseg
        mov     ds, ax

        mov     bx, offset x
        mov     cx, 8

        ; Инициализируем y значением 0
        mov     ax, 0
        mov     y, ax

        ; Передаём параметры в стек (адрес массива и его длину)
        push    bx
        push    cx
        call    is_there_null

end_proc:
        ret
main endp

is_there_null proc near
        push    bp
        mov     bp, sp

        mov     bx, [bp+6]   ; Получаем адрес массива
        mov     cx, [bp+4]   ; Получаем длину массива

loop_start:
        mov     dx, [bx]     ; Загружаем текущий элемент массива
        cmp     dx, 0
        je      set_y_to_one ; Если элемент равен 0, переходим к установке y = 1
        add     bx, 2        ; Переход к следующему элементу
        loop    loop_start   ; Уменьшаем CX и повторяем цикл, если CX не ноль
        jmp     exit_proc    ; Выход, если нулевых элементов не найдено

set_y_to_one:
        mov     ax, 1
        mov     y, ax        ; Присваиваем y значение 1

exit_proc:
        pop     bp
        ret 4                ; Освобождаем параметры из стека
is_there_null endp

cseg ends
        end main