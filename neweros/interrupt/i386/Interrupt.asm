include asm.inc

.code
extern _commonIntHandler:proc

public _trapDivideZero
public _trapDebug
public _trapNMI
public _trapBreakPoint
public _trapOverflow
public _trapBounds
public _trapInvalidOp
public _trapDeviceNotAvailable
public _trapDoubleFault
public _trapCoprocSegOverrun
public _trapInvalidTSS
public _trapSegNotPresent
public _trapStackFault
public _trapProtection
public _trapPageFault
public _trapCoprocErr

_trapDivideZero:
    push 0
    push 0
    jmp commonInt

_trapDebug:
    push 0
    push 1
    jmp commonInt

_trapNMI:
    push 0
    push 2
    jmp commonInt

_trapBreakPoint:
    push 0
    push 3
    jmp commonInt

_trapOverflow:
    push 0
    push 4
    jmp commonInt

_trapBounds:
    push 0
    push 5
    jmp commonInt

_trapInvalidOp:
    push 0
    push 6
    jmp commonInt

_trapCoprocSegOverrun:
    push 0
    push 9
    jmp commonInt

_trapDeviceNotAvailable:
    push 7
    jmp commonInt

_trapDoubleFault:
    push 8
    jmp commonInt

_trapInvalidTSS:
    push 10
    jmp commonInt

_trapSegNotPresent:
    push 11
    jmp commonInt

_trapStackFault:
    push 12
    jmp commonInt

_trapProtection:
    push 13
    jmp commonInt

_trapPageFault:
    push 14
    jmp commonInt

_trapCoprocErr:
    push 16
    jmp commonInt

commonInt:
    pushad
    push ds
    push es
    push fs
    push gs
    
    mov eax, KernelData
    mov ds, ax
    mov es, ax
    mov eax, UserData
    mov fs, ax
    mov gs, ax

    push esp
    call _commonIntHandler
    add esp, 4

retFromInt:

    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret

end