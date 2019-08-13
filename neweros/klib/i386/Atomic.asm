include asm.inc

.code32

public _atoAdd

_atoAdd proc C _val, _par
    mov eax, _par
    mov ebx, _val
    lock add dword ptr ds:[ebx], eax
    ret
_atoAdd endp

.endcode32
end