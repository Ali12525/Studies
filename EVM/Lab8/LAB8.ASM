stseg   segment para stack
        dw      100h dup(?)
stseg   ends

dseg    segment para
curCount    dw      0                                                           ; текущий делитель для PIT (частота тона)
toneTable   dw      5000, 4500, 4000, 3600, 3200, 2800, 2400, 2000, 1600, 1200  ; таблица значений делителя для цифр '0'..'9'
dseg    ends

cseg    segment para public 'CODE'
        assume  cs:cseg, ds:dseg, ss:stseg

main    proc far
    push    ds
    xor     ax, ax
    push    ax                
    mov     ax, dseg
    mov     ds, ax

key_loop:
    ; Ждём нажатия клавиши (возвращает ASCII в AL)
    call    WaitKey           
    cmp     al, 27             ; если ESC (код 27), выходим
    je      exit_prog

    ; Проверяем, что введённый символ — цифра '0'..'9'
    cmp     al, '0'            ; сравниваем с ASCII '0' (48)
    jb      key_loop           ; ниже '0' — не цифра, повторяем ожидание
    cmp     al, '9'            ; сравниваем с ASCII '9' (57)
    ja      key_loop           ; выше '9' — не цифра, повторяем

    ; Преобразуем ASCII-код цифры в индекс 0–9
    mov     bl, al
    sub     bl, '0'            ; BL = AL - '0'
    xor     bh, bh             ; BH = 0, теперь BX = индекс цифры

    ; Получаем значение делителя из toneTable[index]
    lea     si, toneTable      ; SI = адрес начала таблицы
    mov     ax, bx
    shl     ax, 1              ; умножаем индекс на 2 (слово = 2 байта)
    add     si, ax             ; SI = адрес нужного элемента
    mov     ax, [si]           ; AX = toneTable[index]
    mov     [curCount], ax     ; сохраняем в curCount

    ; Запускаем воспроизведение тона
    call    PlayTone

stop_wait:
    ; Ждём нажатия любой клавиши для остановки звука
    call    WaitKey
    call    StopTone

    ; Если нажали ESC — выходим, иначе — к новому вводу цифры
    cmp     al, 27
    je      exit_prog
    jmp     key_loop

exit_prog:
    pop     ds
    mov     ah, 4Ch
    int     21h
main    endp

; WaitKey: ожидает нажатия клавиши через BIOS INT 16h, AH=00
; Выход: ASCII-код символа в AL
WaitKey proc near
    mov     ah, 00h
    int     16h
    ret
WaitKey endp

; PlayTone: настраивает канал 2 PIT для генерации квадратной волны (режим 3)
; Использует делитель из [curCount]
PlayTone proc near
    mov     ax, [curCount]
    mov     dx, ax             ; сохраняем делитель в DX

    ; Отправляем команду на программирование канала 2: режим 3, LSB/MSB
    mov     al, 0B6h            ; Команда для PIT: канал 2 (используется для звука), режим 3 (квадратная волна), LSB+MSB
    out     43h, al             ; Отправляем команду в управляющий регистр PIT (порт 43h)

    ; Передаём младший байт делителя
    mov     al, dl
    out     42h, al
    ; Передаём старший байт делителя
    mov     al, dh
    out     42h, al
    ; 42h — порт данных канала 2 таймера. PIT будет использовать загруженный делитель для генерации нужной частоты.

    ; Включаем динамик: устанавливаем биты 0 и 1 порта 61h
    in      al, 61h
    or      al, 03h
    out     61h, al
    ;Порт 61h управляет динамиком на материнской плате.
    ;Бит 0 — управление динамиком (вкл/выкл)
    ;Бит 1 — разрешение сигнала с канала 2 таймера на динамик

    ret
PlayTone endp

; StopTone: отключает динамик, очищая биты 0 и 1 порта 61h
StopTone proc near
    in      al, 61h
    and     al, 0FCh            ; сбрасываем биты 0 и 1
    out     61h, al
    ret
StopTone endp

cseg    ends
        end main