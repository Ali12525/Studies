stseg   segment para stack
        dw      256 dup(?)
stseg   ends

;--------------------------------------------------------------
; Сегмент данных
;--------------------------------------------------------------
dseg    segment para
; Сообщение меню
menuMsg       db 13,10,'Select a program:',13,10,  \
                '1 - Input ASCII and scan codes',13,10,  \
                '2 - Echo input with output',13,10,       \
                '3 - Calculate average birth date',13,10, \
                '0 - Exit',13,10,'$'

; Данные для ЗАДАНИЯ 1
task1Msg1  db 13,10,'Input letters.',13,10,'Press Enter to stop.$'  ; приглашение для ввода символов
task1Msg2  db 13,10,'ASCII codes: $'                                ; заголовок перед выводом ASCII-кодов
task1Msg3  db 13,10,'Scan-codes:  $'                                ; заголовок перед выводом scan-кодов
asciiBuf   db 50 dup(0)                                             ; буфер хранения введённых ASCII-кодов, максимум 50 символов
scanBuf    db 50 dup(0)                                             ; буфер хранения соответствующих scan-кодов
task1_count dw 0                                                    ; число реально введённых символов

; Данные для ЗАДАНИЯ 2
task2Msg   db 13,10,'Insert text (Esc to stop): $'                  ; приглашение для эхо-ввода до Esc

; Данные для ЗАДАНИЯ 3
prompt1    db 'Hosseyni: $'                                           ; метка перед вводом первой фамилии
promptDate1   db 13,10,'Enter birth date (format DD.MM.YYYY): ','$' ; приглашение ввода даты 1
prompt2    db 'Zaharov: $'                                          ; метка перед вводом второй фамилии
promptDate2   db 13,10,'Enter birth date (format DD.MM.YYYY): ','$' ; приглашение ввода даты 2
days       db 2 dup(0)                                              ; массив дней двух дат (день1, день2)
months     db 2 dup(0)                                              ; массив месяцев двух дат (месяц1, месяц2)
years      dw 2 dup(0)                                              ; массив годов двух дат (год1, год2)
date_buffer db 11,0,11 dup(0)                                       ; DOS-буфер для функции 0Ah: [макс длина=11][фактич длина][11 байт ввода]
result_str db 'birthday: $'                                         ; префикс перед выводом результата
birthday_day db '00.00.0000$'                                       ; строка для вывода вычисленной средней даты (dd.mm.yyyy)
year_str   db '0000$'                                               ; временный буфер для форматирования года
newline    db 0Dh,0Ah,'$'                                           ; CR+LF для новой строки
count      dw 0                                                     ; счётчик введённых дат (0 или 1)


dseg    ends

;--------------------------------------------------------------
; Сегмент кода
;--------------------------------------------------------------
cseg    segment para
        assume cs:cseg, ds:dseg, ss:stseg

;--------------------------------------------------------------
; Точка входа
;--------------------------------------------------------------
main    proc far
        push    ds
        xor     ax, ax
        push    ax
        mov     ax, dseg
        mov     ds, ax

MenuLoop:
        ; вывод меню через DOS int 21h, AH=09h (печать строк до '$')
        mov     dx, offset menuMsg
        mov     ah,09h
        int     21h

        ; считывание выбора: DOS int 21h, AH=01h (ожидание клавиши, AL=ASCII)
        mov     ah, 01h
        int     21h            ; результат в AL
        ; Преобразуем ввод из ASCII в число
        sub     al, '0'
        
        ; Если введён 0, выполняем переход к выходу через промежуточную метку
        cmp     al, 0
        jne     ProcessOption
        jmp short ExitHelper   ; короткий переход к метке, расположенной рядом

ProcessOption:
        cmp     al, 1
        je      RunTask1
        cmp     al, 2
        je      RunTask2
        cmp     al, 3
        je      RunTask3
        jmp     MenuLoop       ; если введено некорректное число, возвращаем меню

ExitHelper:
        ; Используем near‑jmp для перехода к метке ExitProg
        jmp ExitProg

RunTask1:
        call    Task1         ; задание 1
        jmp     MenuLoop

RunTask2:
        call    Task2         ; задание 2
        jmp     MenuLoop

RunTask3:
        call    Task3         ; задание 3
        jmp     MenuLoop

;--------------------------------------------------------------
; Задание 1: Ввод фамилии с выводом ASCII и scan‑кодов
;--------------------------------------------------------------
Task1 proc near
        xor     si, si       ; индекс для буферов

        ; приглашение: DOS int 21h, AH=09h
        mov     dx, offset task1Msg1
        mov     ah,09h
        int     21h

Task1_ReadLoop:
        mov     ah,00h      ; AH=00h — получить клавишу без эха (AL=ASCII, AH=scan)
        int     16h         
        cmp     al,13       ; если Enter – завершаем ввод
        je      Task1_ShowResults
        mov     asciiBuf[si], al    ; сохранить ASCII
        mov     scanBuf[si], ah     ; сохранить scan-код
        inc     si
        cmp     si,50
        jb      Task1_ReadLoop

Task1_ShowResults:
        mov     task1_count, si

        ; Вывод ASCII кодов
        mov     dx, offset task1Msg2
        mov     ah,09h
        int     21h
        mov     cx, task1_count
        xor     si, si
Task1_ShowAscii:
        mov     dl, asciiBuf[si]
        mov     ah,02h          ; DOS int 21h, AH=02h — вывод символа DL
        int     21h
        inc     si
        loop    Task1_ShowAscii

        ; вывод scan-кодов: DOS int 21h, AH=09h для заголовка
        mov     dx, offset task1Msg3
        mov     ah,09h
        int     21h
        mov     cx, task1_count
        xor     si, si
Task1_ShowScan:
        mov     al, scanBuf[si]
        call    PrintHex            ; напечатать hex через BIOS int 21h/AH=02h внутри PrintDigit
        mov     dl, ' '
        mov     ah,02h
        int     21h
        inc     si
        loop    Task1_ShowScan

        ; пауза: BIOS int 16h, AH=00h — ждать любую клавишу
        mov     ah,00h
        int     16h
        ret
Task1 endp

; Вспомогательная процедура для печати числа в 2-байтовом шестнадцатиричном формате
PrintHex proc near
        push    ax
        push    cx
        mov     ah, 0
        mov     cl, 16
        div     cl          ; старший = AH, младший = AL
        call    PrintDigit
        mov     al, ah
        call    PrintDigit
        pop     cx
        pop     ax
        ret
PrintHex endp

PrintDigit proc near
        push    ax
        cmp     al, 10
        jb      Digit
        add     al, 7       ; A-F
Digit:  add     al, '0'
        mov     dl, al
        mov     ah, 02h     ; DOS int 21h, AH=02h — вывод DL
        int     21h
        pop     ax
        ret
PrintDigit endp

;--------------------------------------------------------------
; Задание 2: Эхо‑ввод с завершением по Esc
;--------------------------------------------------------------
Task2 proc near
        ; Вывод приглашения
        mov     dx, offset task2Msg
        mov     ah,09h
        int     21h

Task2_InputLoop:
        mov     ah,00h      ; BIOS int 16h, AH=00h — считать клавишу
        int     16h         
        cmp     al,27       ; если Esc (код 27h), выходим
        je      Task2_Exit
        mov     ah,0Eh      ; вывод символа (BIOS)
        int     10h
        jmp     Task2_InputLoop

Task2_Exit:
        ; Ожидание нажатия клавиши перед возвратом в меню. DOS int 21h, AH=08h — считать символ без эха (чтобы пауза была)
        mov     ah,08h
        int     21h
        ret
Task2 endp

;--------------------------------------------------------------
; Задание 3: Вычисление "средней" даты из двух вводимых дат
;--------------------------------------------------------------
Task3 proc near
        ; Сброс счётчика дат
        mov     word ptr [count], 0

        ; Ввод первой даты
		mov dx, OFFSET promptDate1
		mov ah, 09h                 ; DOS int 21h, AH=09h — вывод строки
		int 21h
		
        mov     dx, offset prompt1
        mov     ah,09h
        int     21h
        call    input_date          ; DOS int 21h, AH=0Ah — buffered input
        call    parse_date          ; парсинг DD.MM.YYYY

        ; Ввод второй даты
		mov dx, OFFSET promptDate2
		mov ah, 09h
		int 21h
		
        mov     dx, offset prompt2
        mov     ah,09h
        int     21h
        call    input_date
        call    parse_date

        ; Вычисление среднего значения дат
        call    compute_average

        ; Вывод результата
        mov     dx, offset result_str
        mov     ah,09h
        int     21h

        mov     dx, offset birthday_day
        mov     ah,09h
        int     21h

        ; Ожидание нажатия клавиши перед возвратом в меню
        mov     ah,08h
        int     21h
        ret
Task3 endp

input_date proc near
        call    clear_keyboard          ; очистка ввода BIOS
        mov     dx, offset date_buffer
        mov     ah, 0Ah                 ; DOS int 21h, AH=0Ah — ввод строки с клавиатуры
        int     21h
        mov     dx, offset newline
        mov     ah, 09h                 ; DOS int 21h, AH=0Ah — вывод строки на экран
        int     21h
        ret
input_date endp

parse_date proc near
        mov     di, [count]
        mov     si, offset date_buffer + 2

        ; Парсинг дня (дд)
        call    atoi_2
        mov     days[di], al
        add     si, 3           ; пропустить точку

        ; Парсинг месяца (мм)
        call    atoi_2
        mov     months[di], al
        add     si, 3           ; пропустить точку

        ; Парсинг года (гггг)
        call    atoi_4
        shl     di, 1
        mov     years[di], ax
        shr     di, 1
        inc     word ptr [count]
        ret
parse_date endp

clear_keyboard proc near
        push    ax
clear_loop:
        mov     ah, 01h         ; BIOS int 16h, AH=01h — проверить, есть ли символ (ZF=1 если нет)
        int     16h
        jz      buffer_empty
        mov     ah, 00h
        int     16h             ; BIOS int 16h, AH=00h — считать символ
        jmp     clear_loop
buffer_empty:
        pop     ax
        ret
clear_keyboard endp

atoi_2 proc near                ; конвертация 2 ASCII-цифр в число
        xor     ax, ax
        mov     al, [si]        ; первая цифра
        sub     al, '0'
        mov     bx, 10
        mul     bx              ; AX = AL * 10
        mov     bl, [si+1]      ; вторая цифра
        sub     bl, '0'
        add     al, bl
        ret
atoi_2 endp

atoi_4 proc near
        xor     ax, ax
        mov     cx, 4
atoi4_loop:
        mov     bx, 10
        mul     bx              ; AX = AX * 10
        mov     bl, [si]
        sub     bl, '0'
        xor     bh, bh
        add     ax, bx
        inc     si
        loop    atoi4_loop
        ret
atoi_4 endp

compute_average proc near
        ; Вычисление среднего года
        mov     ax, years[0]
        add     ax, years[2]
        shr     ax, 1           ; (y1+y2)/2
        mov     bx, ax          ; сохраняем средний год в BX

        ; Очистка year_str перед форматированием
        mov     di, offset year_str
        mov     cx, 4
clear_year_str:
        mov     byte ptr [di], '0'
        inc     di
        loop    clear_year_str

        ; Восстанавливаем значение AX из BX перед форматированием
        mov     ax, bx
        call    format_4_digits     ; разделение на цифры

        ; Установка ES для корректного копирования
        mov     ax, ds
        mov     es, ax

        ; Копирование отформатированного года из year_str в birthday_day (начиная с позиции 6)
        mov     si, offset year_str
        mov     di, offset birthday_day+6
        mov     cx, 4
        rep     movsb       ; копирование ASCII-года


        ; Вычисление среднего дня
        xor     ax, ax
        mov     al, days[0]
        add     al, days[1]
        shr     ax, 1               ; (d1+d2)/2
        call    format_2_digits     ; две цифры в AX -> AH,AL ASCII
        mov     [birthday_day], ah
        mov     [birthday_day+1], al

        ; Вычисление среднего месяца
        xor     ax, ax
        mov     al, months[0]
        add     al, months[1]
        shr     ax, 1               ; (m1+m2)/2
        call    format_2_digits
        mov     [birthday_day+3], ah
        mov     [birthday_day+4], al

        ret
compute_average endp

format_2_digits proc near
        mov     bl, 10
        div     bl              ; деление AX/10 -> AH=остаток, AL=частное
        add     ax, '00'        ; преобразование в ASCII
        xchg    al, ah          ; меняем порядок цифр
        ret
format_2_digits endp

format_4_digits proc near
        mov     si, offset year_str + 3  ; заполняем с конца
        mov     cx, 4
        mov     bx, 10
format4_loop:
        xor     dx, dx
        div     bx              ; деление: DX:AX / BX
        add     dl, '0'
        mov     [si], dl
        dec     si
        loop    format4_loop
        ret
format_4_digits endp

;--------------------------------------------------------------
; Завершение программы
;--------------------------------------------------------------
ExitProg:
        mov     ax,4C00h
        int     21h
main    endp

cseg    ends
        end main
