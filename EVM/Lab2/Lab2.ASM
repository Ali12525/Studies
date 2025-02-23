stseg   segment para stack
        dw 16 dup(?)
stseg   ends

dseg    segment para
B       dw -9
A       dw 7
C       dw 14
Y       dw ?
dseg    ends

cseg    segment para
lab2    proc far
        assume cs:cseg, ds:dseg, ss:stseg
        push    ds
        mov     ax, dseg
        mov     ds, ax

        mov     bx, offset B
        mov     ax, [bx+2]
        mov     dx, [bx]
        and     ax, dx
        mov     cx, [bx+4]
        add     cx, 6
        sub     ax, cx
        mov     Y, ax

        pop     ds
        ret
lab2    endp
cseg    ends

        end lab2
	
