PACKET_BLOCK    = 2
PACKET_OFFSET   = 4
PACKET_SEG      = 6
PACKET_LBA_LOW  = 8
PACKET_LBA_HIG  = 0ch


disk_addr_packet    db  10h            ;结构大小16     +0
                    db  0               ;保留           +1
                    db  1               ;要传输的块数   +2
                    db  0               ;保留           +3
                    dw  0               ;目标地址偏移   +4
                    dw  0               ;目标地址段     +6
                    dd  0               ;LBA低32位      +8    
                    dd  0               ;LBA高32位      +c


readsector proc near

    xor ebx, ebx
    mov ah, 42h
    mov dl, 80h
    mov si, offset disk_addr_packet
    int 13h
    
    jc err

    mov ax, word ptr ds:[disk_addr_packet + PACKET_SEG]
    mov es, ax
    mov bx, word ptr ds:[disk_addr_packet + PACKET_OFFSET]

    ret
err:
    jmp $
readsector endp