ORG 0
    JMP MAIN

ORG 3
    JMP ISR_INT0

ORG 30h
MAIN:
    MOV  R0, #049h    ; начальный адрес для записи
    MOV  R4, #10      ; количество измерений\
	CLR P0.7

    SETB IT0          ; INT0 — фронт
    SETB EX0          ; разрешить INT0
    SETB EA           ; глобально разрешить прерывания

MAIN_LOOP:
WAIT_IDLE:  JNB P2.7, WAIT_IDLE   ; ждём перехода в 1
WAIT_FALL:  JB  P2.7, WAIT_FALL   ; ждём перехода в 0
WAIT_IDLE2:  JNB P2.7, WAIT_IDLE2   ; ждём перехода в 1

    CLR P3.6            ; WR = 0
    ACALL DELAY_SHORT
    SETB P3.6           ; WR = 1  (конец импульса -> старт преобразования)

    DJNZ R4, MAIN_LOOP
    SJMP $

; обработчик прерывания INT0
ISR_INT0:
    CLR  P3.7          ; RD = 0
    MOV  A, P2         ; читаем данные АЦП
    SETB P3.7          ; RD = 1

	MOV P1, A
    MOV  @R0, A        ; сохраняем в RAM
    INC  R0
    RETI

DELAY_SHORT:
    MOV  R2, #0FFh
DLS1:
    DJNZ R2, DLS1
    RET
