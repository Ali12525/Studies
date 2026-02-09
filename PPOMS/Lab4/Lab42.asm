START:
        MOV R0, #049h
        MOV R4, #10
		CLR P0.7

MAIN_LOOP:
WAIT_IDLE:  JNB P2.7, WAIT_IDLE   ; ждём перехода в 1
WAIT_FALL:  JB  P2.7, WAIT_FALL   ; ждём перехода в 0

        MOV R5, #0       ; R5 = нижняя граница (0)
        MOV R6, #0FFh    ; R6 = верхняя граница (255)
        MOV R7, #08      ; 8 итераций

HALV_LOOP:
        MOV A, R6        ; A = верхняя граница
        ADD A, R5        ; A = верхняя граница + нижняя граница
        RRC A            ; A = (верхняя граница+нижняя граница)/2
        MOV P1, A        ; подать mid в DAC
        ACALL DELAY_SHORT

        JNB P3.7, HALV_SET_HI  ; если P3.7 == 0 => Uцап >= Uвх => верхняя граница = mid
        MOV R5, A              ; нижняя граница = mid
        SJMP HALV_NEXT

HALV_SET_HI:
        MOV R6, A

HALV_NEXT:
        DJNZ R7, HALV_LOOP
        MOV @R0, A
        INC R0
        DJNZ R4, MAIN_LOOP

        SJMP $

DELAY_SHORT:
        MOV R2, #0FFh
DLS1:   DJNZ R2, DLS1
        RET
