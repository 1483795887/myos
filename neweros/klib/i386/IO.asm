include asm.inc
include x86Common.inc

.code

public In8
public In16
public In32
public Out8
public Out16
public Out32

In8 proc C port
	push edx
	mov edx, port
	xor eax, eax
	in al, dx
	pop edx
	ret
In8 endp

In16 proc C port
	push edx
	mov edx, port
	xor eax, eax
	in ax, dx
	pop edx
	ret
In16 endp

In32 proc C port
	push edx
	mov edx, port
	xor eax, eax
	in eax, dx
	pop edx
	ret
In32 endp

Out8 proc C port, value
	push edx
	push eax
	mov edx, port
	mov eax, value
	out dx, al
	pop eax
	pop edx
	ret
Out8 endp

Out16 proc C port, value
	push edx
	push eax
	mov edx, port
	mov eax, value
	out dx, ax
	pop eax
	pop edx
	ret
Out16 endp

Out32 proc C port, value
	push edx
	push eax
	mov edx, port
	mov eax, value
	out dx, eax
	pop eax
	pop edx
	ret
Out32 endp

end