START:
        ACALL INIT_TABLE
MAIN:
        MOV A, P2
        ACALL DISPLAY
        SJMP MAIN

INIT_TABLE:
        MOV R0, #069H
        MOV R1, #10H
        MOV DPTR, #DATA_TABLE  ; Указатель на таблицу в ПЗУ

FILL:
        CLR A                   ; Очистка аккумулятора перед загрузкой
        MOVC A, @A+DPTR         ; Чтение байта из ПЗУ
        MOV @R0, A              ; Запись в ОЗУ
        INC DPTR                ; Переход к следующему элементу таблицы
        INC R0                  ; Переход к следующей ячейке массива в ОЗУ
        DJNZ R1, FILL
        RET

DISPLAY:
        PUSH ACC             ; Сохраняем значение аккумулятора

        ; Младшая тетрада
        ANL A, #0FH          ; Оставляем младшие 4 бита
        MOV R0, #069H
        ADD A, R0
        MOV R0, A
        MOV A, @R0           ; Получаем 7-сегментный код
        SETB P3.3            
        CLR P3.4
        MOV P1, A
        ACALL DELAY

        ; Старшая тетрада
        POP ACC              ; Восстанавливаем исходное значение
        SWAP A               ; Меняем старшую и младшую тетрады
        ANL A, #0FH
        MOV R0, #069H
        ADD A, R0
        MOV R0, A
        MOV A, @R0
        CLR P3.3
        SETB P3.4           
        MOV P1, A
        ACALL DELAY

        RET

DELAY:
        MOV R2, #0FFH
D1:     DJNZ R2, D1
        RET

DATA_TABLE:
        DB 11000000B ; 0
        DB 11111001B ; 1
        DB 10100100B ; 2
        DB 10110000B ; 3
        DB 10011001B ; 4
        DB 10010010B ; 5
        DB 10000010B ; 6
        DB 11111000B ; 7
        DB 10000000B ; 8
        DB 10010000B ; 9
        DB 10001000B ; A
        DB 10000011B ; B
        DB 11000110B ; C
        DB 10100001B ; D
        DB 10000110B ; E
        DB 10001110B ; F

        END
