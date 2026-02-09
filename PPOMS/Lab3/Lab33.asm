start:
	JB P2.7, OFF         ; Проверка 8-го датчика (бит 7 порта 2)

ON:
	CLR P3.3
	SETB P3.4
 	MOV P1,#11000000b    ; 'O'
 	MOV P1,#0FFh

	SETB P3.3
	CLR P3.4
 	MOV P1,#10101011b    ; 'n'
 	MOV P1,#0FFh
 	jmp start

OFF:
 	CLR P3.3
 	SETB P3.4
 	MOV P1,#11000000b    ; 'O'
 	MOV P1,#0FFh

 	SETB P3.3
 	CLR P3.4
 	MOV P1,#10001110b    ; 'F'
 	CLR P3.3
 	MOV P1,#0FFh
 	jmp start