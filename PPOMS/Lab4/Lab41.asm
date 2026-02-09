START:
        MOV R0, #049h
        MOV R4, #10
		CLR P0.7

MAIN_LOOP:
WAIT_IDLE:  JNB P2.7, WAIT_IDLE   ; ждём перехода в 1
WAIT_FALL:  JB  P2.7, WAIT_FALL   ; ждём перехода в 0

        CLR A              ; A - результат
        MOV R1, #080h      ; маска, 1000 0000b

SAR_LOOP:
        MOV B, A           ; B = текущий результат
        ORL A, R1          
        MOV P1, A          
        ACALL DELAY_SHORT

        JNB P3.7, SAR_NOSET  ; если P3.7 == 0 (Uцап >= Uвх) — не устанавливаем бит
        SJMP SAR_NEXT

SAR_NOSET:
        MOV A, B           ; откат — не устанавливаем текущий бит

SAR_NEXT:
        ; Логический сдвиг маски R1 вправо (через аккумулятор)
        MOV R3, A          ; сохранить текущий результат (A) во временный регистр R3
        MOV A, R1          ; A = маска
        CLR C
        RRC A              ; логический сдвиг вправо
        MOV R1, A          ; сохранить обратно в R1 (новая маска)
        MOV A, R1          ; поместить маску в A для проверки нуля
        JNZ SAR_CONTINUE   ; если маска != 0 => продолжаем цикл
        MOV A, R3          ; восстановить результат (A = итог)
        SJMP SAR_DONE_MASK ; маска = 0 => завершаем
SAR_CONTINUE:
        MOV A, R3          ; восстановить результат (A)
        SJMP SAR_LOOP

SAR_DONE_MASK:
        MOV @R0, A         
        INC R0
        DJNZ R4, MAIN_LOOP
        SJMP $

DELAY_SHORT:
        MOV R2, #0FFh
DLS1:   DJNZ R2, DLS1
        RET
