include asm.inc
include x86Common.inc

.code16
jmp mbr_start


INDEX     = 1beh
CANBOOT   = 0h
STARTSECT = 8h

mbr_start:
    mov ax, MBRSEG
    mov es, ax
    mov ax, BOOTSEG
    mov ds, ax
    xor di, di
    xor si, si
    mov cx, 100h
    rep movsw

    .jmpl MBRSEG, next

next:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0fffch

    mov bx, INDEX
    mov cx, 4
findloop:
    cmp byte ptr [bx + CANBOOT], 80h
    jz short boot
    add bx, 10h
    loop findloop
    jmp error
boot:
    mov word ptr ds:[disk_addr_packet + PACKET_OFFSET], 0
    mov word ptr ds:[disk_addr_packet + PACKET_SEG],    BOOTSEG
    mov eax, ds:[bx + STARTSECT]
    mov dword ptr ds:[disk_addr_packet + PACKET_LBA_LOW], eax

    mov ds:[0], eax     ;开始扇区
    call readsector
    .jmpl BOOTSEG,0000h
    jmp $

MessageLength   = 7
ErrorMessage db "NO BOOT"

error:
    mov bp, offset ErrorMessage
    mov ax, ds
    mov es, ax
    mov cx, MessageLength
    mov ax, 1301h
    mov bx, 0007h
    mov dl, 0
    int 10h
    jmp $

include sectreader.inc

.endcode16
end