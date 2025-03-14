stseg   segment para stack
        dw 16 dup(?)
stseg   ends

dseg    segment para
x       dw 1, 0, 1, 2, 3, 0, 4, 5
y       dw ?                 
dseg    ends

cseg    segment para
lab3    proc far
        assume cs:cseg, ds:dseg, ss:stseg
        push    ds
        mov     ax, dseg
        mov     ds, ax

        mov     bx, offset x

        ; Инициализируем y значением 0
        mov     ax, 0
        mov     y, ax

        mov     cx, 8 ; Устанавливаем счетчик итераций

loop_start:
        mov     dx, [bx]      ; Загружаем текущий элемент массива
        cmp     dx, 0         
        je      set_y_to_one  ; Если элемент равен 0, то в переходим в set_y_to_one
        add     bx, 2         ; Переходим к следующему элементу (слово = 2 байта)
        loop    loop_start
        jmp     end_proc

set_y_to_one:		      ;y = 1
        mov     ax, 1
        mov     y, ax
        jmp end_proc

end_proc:
        pop     ds
        ret
lab3    endp
cseg    ends
        end lab3
