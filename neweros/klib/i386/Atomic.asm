include asm.inc

.code

public atomicAdd

atomicAdd proc C _val, _par
    push eax
    push ebx
    mov eax, _par
    mov ebx, _val
    lock add dword ptr ds:[ebx], eax
    pop ebx
    pop eax
    ret
atomicAdd endp

end