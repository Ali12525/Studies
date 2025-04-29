stseg   segment para stack
        dw      100h dup(?)       ; резерв 256 слов для стека
stseg   ends

dseg    segment para
curDiv      dw      0             ; текущий делитель для PIT (частота тона)
durCount    dw      0             ; счётчик длительности текущей ноты (итерации delayStep)
pauseCount  dw      60000         ; счётчик длительности паузы (~1 секунда)

; Таблица делителей для нот C, D, E, F, G, A, B
divTable    dw      5000,4500,4000,3600,3200,2800,2400

; Последовательность мелодии: индексы в divTable или 0FFh для «длинной» паузы
melodyBytes db      6,3,6,3,6,5,5,0FFh
            db      5,3,5,3,5,6,6,0FFh
            db      6,3,6,3,6,5,5,0FFh
            db      5,3,5,3,5,4,0FFh
            ; --- припев ---
            db      4,5,5,5,5,0FFh
            db      4,5,5,5,5,0FFh
            db      5,6,5,6,5,6,0FFh
            db      4,5,5,5,5,0FFh
            db      4,5,5,5,5,0FFh
            db      5,6,5,4,3,4,0FFh

totalNotes  equ     69            ; общее число байт в melodyBytes

defaultDur  dw      50000         ; стандартная длительность каждой ноты (~2 секунды)
dseg    ends

cseg    segment para public 'CODE'
    assume cs:cseg, ds:dseg, ss:stseg

main    proc far
    push    ds
    xor     ax, ax
    push    ax                 
    mov     ax, dseg
    mov     ds, ax

    ; Установка длительности нот и пауз
    mov     ax, [defaultDur]
    mov     [durCount], ax     ; загрузить начальную длительность ноты
    mov     ax, [pauseCount]
    mov     [pauseCount], ax   ; (может быть повторно инициализировано)

    xor     si, si             ; SI = 0, индекс текущей ноты в melodyBytes

play_sequence:
    ; Чтение следующего байта из melodyBytes
    mov     al, [melodyBytes + si]
    cmp     al, 0FFh           ; проверка на маркер длинной паузы
    je      only_pause         ; если 0FFh — перейти к длинной паузе

    ; Вычисление делителя для текущей ноты
    mov     bl, al             ; BL = индекс в divTable
    xor     bh, bh             ; BH = 0
    lea     di, divTable       ; DI = адрес начала таблицы делителей
    mov     ax, bx             ; AX = индекс
    shl     ax, 1              ; умножаем на 2 (размер WORD)
    add     di, ax             ; DI -> divTable[index]
    mov     ax, [di]           ; AX = divTable[index]
    mov     [curDiv], ax       ; сохраняем текущий делитель

    ; Проигрываем ноту заданной длительности
    mov     cx, [durCount]     ; CX = количество итераций delayStep
    call    playToneDuration

    ; Корректная пауза между нотами
    mov     cx, [pauseCount]
    call    silence_period

    jmp     next_note

only_pause:
    ; Выключаем динамик перед длинной паузой
    call    disable_speaker

    ; Длинная пауза в два раза дольше обычной
    mov     cx, [pauseCount]
    call    silence_period
    mov     cx, [pauseCount]
    call    silence_period

next_note:
    inc     si                 ; переходим к следующему байту melodyBytes
    cmp     si, totalNotes     ; проверка конца мелодии
    jb      play_sequence      ; если ещё есть байты — повторяем

    ; Завершение программы: выход в DOS
    pop     ds
    mov     ah, 4Ch
    int     21h
main    endp

; enable_speaker: включить динамик с частотой curDiv
enable_speaker proc near
    mov     ax, [curDiv]       ; загружаем делитель
    mov     dx, ax             ; DX для байтов делителя
    mov     al, 0B6h           ; Команда для PIT: канал 2 (используется для звука), режим 3 (квадратная волна), LSB+MSB
    out     43h, al            ; управляющий регистр PIT
    mov     al, dl             ; младший байт делителя
    out     42h, al            ; порт данных канала 2
    mov     al, dh             ; старший байт делителя
    out     42h, al            ; порт данных канала 2
    in      al, 61h            ; читаем порт управления динамиком
    or      al, 03h            ; включаем биты 0 (динамик) и 1 (PIT)
    out     61h, al
    ret
enable_speaker endp
;Порт 61h управляет динамиком на материнской плате.
;Бит 0 — управление динамиком (вкл/выкл)
;Бит 1 — разрешение сигнала с канала 2 таймера на динамик

; disable_speaker: отключить динамик (сброс битов 0 и 1 порта 61h)
disable_speaker proc near
    in      al, 61h
    and     al, 0FCh
    out     61h, al
    ret
disable_speaker endp

; delayStep: одна итерация задержки (NOP — можно расширить для точного тайминга)
delayStep proc near
    nop
    ret
delayStep endp

; playToneDuration: воспроизвести тон в течение CX итераций delayStep
playToneDuration proc near
    call    enable_speaker
tone_loop:
    call    delayStep
    loop    tone_loop          ; повторять until CX=0
    call    disable_speaker
    ret
playToneDuration endp

; silence_period: пауза без звука, CX итераций delayStep
silence_period proc near
silence_loop:
    call    delayStep
    loop    silence_loop
    ret
silence_period endp

cseg    ends
        end     main