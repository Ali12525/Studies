stseg   segment para stack
    ; Стек (100h слов)
    dw 100h dup (?)
stseg   ends

dseg    segment para
    ; Объявление переменной для строки
    msg db 'Hello, World!', 0
dseg    ends

cseg    segment para
lab61   proc far
    assume cs:cseg, ds:dseg, ss:stseg

    ; Инициализация сегмента данных
    mov ax, dseg
    mov ds, ax

    ; Установка видеорежима 03h (текстовый 80x25)
    mov ax, 0003h
    int 10h

    ; Вывод сообщения через int 10h, функцию 0Eh
    mov si, offset msg
print_loop:
    lodsb                   ; Загрузка символа из DS:SI в AL
    test al, al             ; Проверка на конец строки (нулевой символ)
    jz wait_key             ; Если символ равен нулю, переход к ожиданию ввода
    mov ah, 0Eh             ; Функция BIOS для вывода символа
    mov bh, 0               ; Номер страницы
    int 10h
    jmp print_loop

wait_key:
    ; Дополнительное ожидание нажатия клавиши через int 16h, функцию 00h
    mov ah, 00h
    int 16h

    ; Завершение программы
    mov ax, 4C00h
    int 21h

lab61   endp
cseg    ends
end lab61
