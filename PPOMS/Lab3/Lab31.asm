START:
    MOV  A, P2
    MOV  P1, A

    XRL  A, #00010101b	; инвертировать биты 0,2,4
    MOV  P1, A

    ANL  A, #11101010b	; очистить биты 0, 2, 4
    MOV  P1, A

    ORL  A, #00010101b 	; установить биты 0, 2, 4
    MOV  P1, A
	JMP START
