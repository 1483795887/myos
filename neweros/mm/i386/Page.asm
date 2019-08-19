include asm.inc

.code

public setPageDirectory

setPageDirectory proc C pd
    push eax
    mov eax, pd
    mov cr3, eax
    pop eax
    ret
setPageDirectory endp

end