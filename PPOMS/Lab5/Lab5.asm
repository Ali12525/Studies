START:
        MOV R0, #049h
        MOV R4, #10
        CLR P0.7

MAIN_LOOP:
WAIT_IDLE:  JNB P2.7, WAIT_IDLE   ; ждём перехода в 1
WAIT_FALL:  JB  P2.7, WAIT_FALL   ; ждём перехода в 0
WAIT_IDLE2:  JNB P2.7, WAIT_IDLE2   ; ждём перехода в 1

        CLR P3.6            ; WR = 0
        ACALL DELAY_SHORT
        SETB P3.6           ; WR = 1  (конец импульса -> старт преобразования)

WAIT_CONV:
        JB  P3.2, WAIT_CONV ; ждем, пока INTR не станет 0 (индикатор конца преобразования)

        CLR P3.7            ; RD = 0 Формирование сигнала разрешения чтения АЦП
        MOV A, P2           ; читаем 8-битный результат с шины данных
        SETB P3.7           ; RD = 1  (заканчиваем чтение)

		MOV P1, A
        MOV @R0, A
        INC R0
        DJNZ R4, MAIN_LOOP

        SJMP $         

DELAY_SHORT:
        MOV R2, #0FFh
DLS1:   DJNZ R2, DLS1
        RET
