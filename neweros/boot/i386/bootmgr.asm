include asm.inc
include x86Common.inc
include fat32.inc
;这里只写了fat32的

DIVISION    = 01beh
BOOT        = 00h
ACTIVE      = 80h
STARTSECT   = 08h

.code16
	mov ax, cs
    mov ds, ax
    mov ss, ax
    mov sp, 0fffch
    
    mov ax, TEMPSEG 
    mov es, ax
    xor eax, eax
    mov si, 0
    mov cx, 1
    call readsectors;读取mbr

    mov di, DIVISION
findactionloop:
    cmp byte ptr es:[di + BOOT], ACTIVE
    jz getactivedivision
    cmp di, 496
    jnb noactivedivision
    add di, 010h
    jmp findactionloop

getactivedivision:
    mov eax, es:[di + STARTSECT]
    mov ds:[startSect], eax
    mov bx, DBRSEG
    mov es, bx
    mov si, 0
    mov cx, 1
    call readsectors

    mov al, es:[SectorsPerCluster]
    mov ds:[sectorsPerCluster], al
    mov ax, es:[SavedSectorCount]
    mov ds:[savedSectorCount],  ax
    mov al, es:[FatCount]
    mov ds:[fatCount],          al
    mov eax, es:[SectorCountPerFat]
    mov ds:[sectorCountPerFat], eax
    mov eax, es:[RootClusterIndex]
    mov ds:[rootClusterIndex],  eax

    mov ebx, ds:[startSect]
    add ebx, dword ptr ds:[savedSectorCount]
    mov ds:[fatSectorIndex], ebx

    xor ecx, ecx
    mov cl, ds:[fatCount]
    mov eax,ds:[sectorCountPerFat]
    mul ecx
    
    add eax, ebx
    mov ds:[rootSectorIndex], eax

    

NameLen = 13
kernel              db "KERNEL  EXE "

startSect           dd 0
sectorsPerCluster   db 0
savedSectorCount    dw 0
fatCount            db 0
sectorCountPerFat   dd 0
rootClusterIndex    dd 0

fatSectorIndex      dd 0
rootSectorIndex     dd 0

    

PACKET_BLOCK    = 2
PACKET_OFFSET   = 4
PACKET_SEG      = 6
PACKET_LBA_LOW  = 8
PACKET_LBA_HIG  = 0ch


disk_addr_packet    db  10h            ;结构大小16     +0
                    db  0               ;保留           +1
                    dw  1               ;要传输的块数   +2
                    dw  0               ;目标地址偏移   +4
                    dw  0               ;目标地址段     +6
                    dd  0               ;LBA低32位      +8    
                    dd  0               ;LBA高32位      +c

;cx为读取扇区数,es:si为存放位置 eax为LBA,更新es:si的值
readsectors proc near
    mov word ptr ds:[disk_addr_packet + PACKET_BLOCK], cx
    mov word ptr ds:[disk_addr_packet + PACKET_OFFSET] ,si
    mov word ptr ds:[disk_addr_packet + PACKET_SEG], es
    mov dword ptr ds:[disk_addr_packet + PACKET_LBA_LOW], eax

    mov ah, 42h
    mov dl, 80h
    mov si, offset disk_addr_packet
    int 13h
    
    jc readsectorserr

    mov ax, word ptr ds:[disk_addr_packet + PACKET_SEG]
    mov es, ax
    mov si, word ptr ds:[disk_addr_packet + PACKET_OFFSET]

    ret

readsectors endp

noactivedivision:
    mov cx, 0
    call showerror
    jmp $

readsectorserr:
    mov cx, 1
    call showerror
    jmp $

MessageLength   =   32
ErrorMessage    db  "NO ACTIVE DIVISION             ",0
                db  "READ SECTORS FAILED            ",0
showerror proc near ;cx为编号
    mov ax, MessageLength
    mul cx
	mov	bp, offset ErrorMessage			;
    add bp, ax
	mov	ax, ds			; ES:BP = 字符串地址
	mov	es, ax			; /
	mov	cx, MessageLength	; CX = 长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 光标移动(BH = 0) 黑底白字(BL = 07h)
	mov	dl, 0
	int	10h			; int 10h
showerror endp
.endcode16 
end