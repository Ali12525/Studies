stseg   segment para stack
    ; ���� (100h ����)
    dw 100h dup (?)
stseg   ends

dseg    segment para
    ; ���������� ���������� ��� ������
    msg db 'Hello, World!', 0
dseg    ends

cseg    segment para
lab61   proc far
    assume cs:cseg, ds:dseg, ss:stseg

    ; ������������� �������� ������
    mov ax, dseg
    mov ds, ax

    ; ��������� ����������� 03h (��������� 80x25)
    mov ax, 0003h
    int 10h

    ; ����� ��������� ����� int 10h, ������� 0Eh
    mov si, offset msg
print_loop:
    lodsb                   ; �������� ������� �� DS:SI � AL
    test al, al             ; �������� �� ����� ������ (������� ������)
    jz wait_key             ; ���� ������ ����� ����, ������� � �������� �����
    mov ah, 0Eh             ; ������� BIOS ��� ������ �������
    mov bh, 0               ; ����� ��������
    int 10h
    jmp print_loop

wait_key:
    ; �������������� �������� ������� ������� ����� int 16h, ������� 00h
    mov ah, 00h
    int 16h

    ; ���������� ���������
    mov ax, 4C00h
    int 21h

lab61   endp
cseg    ends
end lab61
