stseg   segment para stack
        dw 16 dup(?)
stseg   ends

dseg    segment para
        x       dw 1, 0, 1, 2, 3, 0, 4, 5
        y       dw ?
dseg    ends

cseg    segment para

main proc far
        ; Инициализация сегмента данных
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

        call    is_there_null

end_proc:
        ret
main endp

is_there_null proc near
loop_start:
        mov     dx, [bx]     ; Загружаем текущий элемент массива
        cmp     dx, 0
        je      set_y_to_one ; Если элемент равен 0, то переходим в set_y_to_one
        add     bx, 2        ; Переход к следующему элементу (одно слово = 2 байта)
        loop    loop_start   ; Уменьшаем CX и повторяем цикл, если CX не равен 0
        ret                  ; Возврат в основную программу

set_y_to_one:
        mov     ax, 1
        mov     y, ax        ; Присваиваем y значение 1
        ret                  ; Возврат в основную программу
is_there_null endp

cseg ends
        end main