stseg segment para stack
        dw 16 dup(?)
stseg ends

dseg segment para
        x       dw 1, 0, 1, 2, 3, 0, 4, 5
        y       dw ?
	old_adress dw 2 dup(?)  ; Для хранения старого вектора прерывания
dseg ends

cseg segment para

main proc far
	assume cs:cseg, ds:dseg, ss:stseg
	push ds
	mov ax, 0
	push ax
        push es
        mov ax, dseg
        mov ds, ax

        ; Сохраняем старый обработчик прерывания
        mov ah, 35h       ; Получаем адрес прерывания 00h
        mov al, 0FEh
        int 21h
        mov old_adress, es
        mov old_adress+2, bx
        push ds

        ; Устанавливаем новый обработчик прерывания
        mov dx, offset is_there_null
        mov ax, seg is_there_null
        mov ds, ax
        mov ah, 25h       ; Устанавливаем обработчик
        mov al, 0FEh
        int 21h
        pop ds

        ; Подготовка параметров для вызова прерывания
        mov bx, offset x    ; Адрес массива
        mov cx, 8           ; Количество элементов
        mov     ax, 0
        mov     y, ax
           

        int 0FEh             ; Вызов нашего обработчика

        ; Восстанавливаем старый обработчик прерывания
        push ds
        mov dx, old_adress+2
        mov ax, old_adress
        mov ds, ax
        mov ah, 25h
        mov al, 0FEh
        int 21h
        pop ds
        pop es

        ret
main endp

is_there_null proc far
        push ax
        push bx
        push cx
        push dx
        push ds

        mov ax, dseg
	mov ds, ax
loop_start:
        mov dx, [bx]     ; Загружаем текущий элемент массива
        cmp dx, 0
        je  set_y_to_one ; Если элемент равен 0, переходим в set_y_to_one
        add bx, 2        ; Переход к следующему элементу
        loop loop_start  ; Уменьшаем CX и повторяем цикл

        jmp end_isr

set_y_to_one:
        mov ax, 1
        mov y, ax        ; Присваиваем y значение 1

end_isr:
        pop ds
        pop dx
        pop cx
        pop bx
        pop ax
        iret             ; Возврат из прерывания
is_there_null endp

cseg ends
        end main