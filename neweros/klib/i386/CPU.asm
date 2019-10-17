include asm.inc
include x86Common.inc

.code

public openPageMode
public setGlobalDescriptorTable
public setInterruptVectorTable
public halt

openPageMode proc C 
    push eax

    mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax

    pop eax
    ret
openPageMode endp

ljmp proc   ;eax为跳转目标,bx为段
    mov ds:[setupoffset], eax
    mov ds:[setupseg], bx

    longjmp             db 0eah
    setupoffset         dd 0
    setupseg            dw 0

    ret
ljmp endp

setGlobalDescriptorTable proc C table
    push eax
    mov eax, table
    mov ds:[GdtAddr], eax
    lgdt fword ptr ds:[GdtPtr]

    mov eax, next
    mov bx, KERNEL_CODE
    call ljmp
next:
    add esp, 4
    pop eax
    ret
setGlobalDescriptorTable endp

setInterruptVectorTable proc C table
    push eax
    mov eax, table
    mov ds:[IdtAddr], eax
    lidt fword ptr ds:[IdtPtr]
    pop eax
    ret
setInterruptVectorTable endp

halt proc C
	hlt
	ret
halt endp

.data

GdtPtr  dw 00fffh
GdtAddr dd 0

IdtPtr  dw 00fffh
IdtAddr dd 0

end
