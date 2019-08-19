include asm.inc

.code

public doOpenPageMode

doOpenPageMode proc C 
    push eax

    mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax

    pop eax
    ret
doOpenPageMode endp

end