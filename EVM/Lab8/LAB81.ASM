stseg   segment para stack
        dw      100h dup(?)
stseg   ends

dseg    segment para
curCount    dw      3000            ; текущий делитель PIT (начальный тон)
stepSize    dw      100             ; шаг изменения делителя при '+'/'-'
dseg    ends

cseg    segment para public 'CODE'
        assume  cs:cseg, ds:dseg, ss:stseg

main    proc far
    push    ds
    xor     ax, ax
    push    ax                
    mov     ax, dseg
    mov     ds, ax

    ; Запускаем тон
    call    PlayTone

adjust_loop:
    ; Ожидаем нажатия клавиши: ASCII-код в AL
    call    WaitKey
    cmp     al, 27            ; ESC (код 27)?
    je      exit_prog         ; если да — выход
    cmp     al, '+'           ; '+' — повысить тон?
    je      increase
    cmp     al, '-'           ; '-' — понизить тон?
    je      decrease
    jmp     adjust_loop       ; остальные клавиши игнорируем

; Увеличение частоты (уменьшаем делитель)
increase:
    mov     ax, [curCount]
    sub     ax, [stepSize]    ; уменьшаем делитель
    cmp     ax, 1             ; не ниже 1
    jb      adjust_loop       ; если ниже — пропускаем
    mov     [curCount], ax    ; сохраняем новый делитель
    call    UpdateTone        ; перенастраиваем PIT
    jmp     adjust_loop

; Уменьшение частоты (увеличиваем делитель)
decrease:
    mov     ax, [curCount]
    add     ax, [stepSize]    ; увеличиваем делитель
    mov     [curCount], ax    ; сохраняем новый делитель
    call    UpdateTone        ; перенастраиваем PIT
    jmp     adjust_loop

; Завершение программы: выключаем тон, восстанавливаем DS, возвращаемся в DOS
exit_prog:
    call    StopTone
    pop     ds
    mov     ah, 4Ch
    int     21h
main    endp

; WaitKey: BIOS int16h AH=00, ожидаем клавишу, возвращаем ASCII в AL
WaitKey proc near
    mov     ah, 00h
    int     16h
    ret
WaitKey endp

; UpdateTone: перенастройка канала 2 PIT без выключения динамика (режим 3)
UpdateTone proc near
    mov     ax, [curCount]    ; загружаем текущий делитель
    mov     dx, ax            ; сохраняем в DX для передачи байтов
    mov     al, 0B6h          ; канал 2, режим 3, передача LSB/MSB
    out     43h, al           ; записываем в управляющий регистр PIT
    mov     al, dl            ; младший байт делителя
    out     42h, al           ; порт данных канала 2
    mov     al, dh            ; старший байт делителя
    out     42h, al           ; порт данных канала 2
    ret
UpdateTone endp

; PlayTone: настройка PIT + включение динамика (порт 61h биты 0 и 1)
PlayTone proc near
    mov     ax, [curCount]
    mov     dx, ax            ; делитель для передачи
    mov     al, 0B6h          ; Команда для PIT: канал 2 (используется для звука), режим 3 (квадратная волна), LSB+MSB
    out     43h, al           ; Отправляем команду в управляющий регистр PIT (порт 43h)
    mov     al, dl            ; младший байт
    out     42h, al           ; в порт 42h
    mov     al, dh            ; старший байт
    out     42h, al           ; в порт 42h
    in      al, 61h           ; читаем порт 61h
    or      al, 03h           ; устанавливаем биты 0 (динамик) и 1 (PIT)
    out     61h, al           ; записываем обратно
    ret
PlayTone endp
;Порт 61h управляет динамиком на материнской плате.
;Бит 0 — управление динамиком (вкл/выкл)
;Бит 1 — разрешение сигнала с канала 2 таймера на динамик

; StopTone: выключение динамика — сброс битов 0 и 1 порта 61h
StopTone proc near
    in      al, 61h
    and     al, 0FCh         ; сбрасываем биты 0 и 1
    out     61h, al
    ret
StopTone endp

cseg    ends
        end main