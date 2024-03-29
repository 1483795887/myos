include asm.inc
include x86Common.inc
include fat32.inc
include vga.inc
include systemdata.inc
;这里只写了fat32的

DIVISION    = 01beh
BOOT        = 00h
ACTIVE      = 80h
STARTSECT   = 08h

.code16
loader:
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
noactivedivision:
    mov cx, 0
    call showerror
    jmp $
getactivedivision:
    mov eax, es:[di + STARTSECT]
    mov ds:[startSect], eax
    mov bx, DBRSEG
    mov es, bx
    mov si, 0
    mov cx, 1
    call readsectors

    xor eax, eax
    mov al, es:[SectorsPerCluster]
    mov ds:[sectorsPerCluster], al
    shl eax, 9; * 512
    mov ds:[sizeOfCluster], eax

    mov ax, es:[SavedSectorCount]
    mov ds:[savedSectorCount],  ax
    mov al, es:[FatCount]
    mov ds:[fatCount],          al
    mov eax, es:[SectorCountPerFat]
    mov ds:[sectorCountPerFat], eax
    mov eax, es:[RootClusterIndex]
    mov ds:[rootClusterIndex],  eax
    mov ds:[dirClusterIndex], eax

    mov ebx, ds:[startSect]
    movzx ecx, word ptr ds:[savedSectorCount]
    add ebx, ecx
    mov ds:[fatSectorIndex], ebx

    xor ecx, ecx
    mov cl, ds:[fatCount]
    mov eax,ds:[sectorCountPerFat]
    mul ecx
    
    add eax, ebx
    mov ds:[firstDataSector], eax

    mov ax, TEMPSEG;    读取FAT
    mov es, ax
    mov si, 0
    mov cx, 1
    mov eax, ds:[fatSectorIndex]
    call readsectors

    mov ax, ROOTDIRSEG
    mov es, ax
    mov si, 0
    mov eax, ds:[dirClusterIndex];读取根目录
    call readcluster

    mov di, offset rootName
    call findfile

    movzx eax, word ptr es:[si + CLUSTERHIG]
    shl eax, 10h
    mov ax, es:[si + CLUSTERLOW]
    mov bx, ROOTDIRSEG
    mov es, bx
    mov si, 0
    call readcluster

    mov di, offset setupName
    mov ax, SETUPSEG
    mov es, ax
    mov si, 0
    call readfile   

    mov di, offset kernelName
    mov ax, KERNELSEG
    mov es, ax
    mov si, 0
    call readfile

    mov di, offset bootFontName
    mov ax, FONTSEG
    mov es, ax
    mov si, 0
    call readfile

    jmp enterIntoPM

    jmp $

    
findfile proc near          ;di为文件名偏移返回si
    push ax
    push es
    push bx

    mov ax, ROOTDIRSEG
    mov es, ax
nextcluser:
    mov esi, 0
findfileloop:
    cmp byte ptr es:[si + FILETYPE], LONGNAME   ;为图方便汇编只读取短文件名文件，
    jz nextfile                                 ;在kernel的fs中再实现长文件名读取
    cmp byte ptr es:[si + NAMEOFF], 0
    jz findfileerror
    mov bx, 0
strcmploop:
    mov al, [bx + di]
    cmp al, es:[bx + si]
    jnz nextfile
    inc bx
    cmp bx, NameLen
    jb strcmploop
    jmp findover
nextfile:
    add si, INODESIZE
    cmp esi, ds:[sizeOfCluster]
    jnz findfileloop
    mov eax, ds:[dirClusterIndex]
    call getNextCluster
    cmp eax, ds:[rootClusterIndex]
    jbe findfileerror
    cmp eax, MAXCLUSTER
    jnb findfileerror
    mov ds:[dirClusterIndex], eax
    mov si, 0
    call readcluster
    jmp nextcluser
findfileerror:
    mov ax, di
    sub ax, offset rootName
    mov bl, NameLen
    div bl
    add ax, 2
    mov cx, ax
    call showerror
    jmp $
findover:
    pop bx
    pop es
    pop ax

    mov ax, si
    ret
findfile endp

getNextCluster proc ;eax为参数返回eax
    push es
    push bx
    push si
    push cx
    mov bx, TEMPSEG
    mov es, bx
    shl eax, 2
    cmp eax, 200h ; *4
    jb thissector
    inc dword ptr ds:[fatSectorIndex]
    mov eax, ds:[fatSectorIndex]
    mov si, 0
    mov cx, 1
    call readsectors
thissector:
    and eax, 1fch   ;余512，与4对齐
    mov eax, es: [eax]
    pop cx
    pop si
    pop bx
    pop es
    ret
getNextCluster endp

readfile proc near  ;读入es:si di 为文件名偏移
    push eax

    push si
    call findfile
    push es
    mov ax, ROOTDIRSEG
    mov es, ax
    movzx eax, word ptr es:[si + CLUSTERHIG]
    shl eax, 10h
    mov ax, es:[si + CLUSTERLOW]
    pop es
    pop si
    
    mov ecx, ds:[sizeOfCluster]
    shr ecx, 4
    and ecx, 0ffffh         ;暂时只能支持簇大小在2^28B以内即128M以内，应该没问题
readfileloop:
    mov si, 0
    push ecx
    call readcluster
    call getNextCluster
    pop ecx
    push ax
    mov ax, es
    add ax, cx
    mov es ,ax
    pop ax
    cmp eax, LASTCLUSTER
    jnz readfileloop

    pop eax
    ret
readloop:
    
readfile endp

NameLen = 11
rootName            db "ROOT       "
setupName           db "SETUP   EXE"
kernelName          db "KERNEL  EXE"
bootFontName        db "BOOT    FNT"

startSect           dd 0
sectorsPerCluster   db 0
savedSectorCount    dw 0
fatCount            db 0
sectorCountPerFat   dd 0
rootClusterIndex    dd 0
sizeOfCluster       dd 0

fatSectorIndex      dd 0
dirClusterIndex     dd 0
firstDataSector     dd 0

    

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

readcluster proc near ;eax为cluster序号不修改 es:si为位置，更新
    push ecx
    push edx
    push eax

    sub eax, ds:[rootClusterIndex]
    movzx ecx, ds:[sectorsPerCluster]
    mul ecx
    add eax, ds:[firstDataSector]

    call readsectors

    pop eax
    pop edx
    pop ecx
    ret
readcluster endp

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

readsectorserr:
    mov cx, 1
    call showerror
    jmp $

readsectors endp





MessageLength   =   32
ErrorMessage    db  "NO ACTIVE DIVISION             ",0
                db  "READ SECTORS FAILED            ",0
                db  "LOST OF ROOT DIR               ",0
                db  "LOST OF SETUP.EXE              ",0
                db  "LOST OF KERNEL.EXE             ",0
                db  "ERROR IN PE                    ",0
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
    ret
showerror endp

enterIntoPM:

    mov si, 0
    mov di, DBR_OFFSET
    mov ax, DBRSEG
    mov ds, ax
    mov ax, TEMPSEG
    mov es, ax
    mov cx, 100h
    rep movsw   ;将dbr保存

    mov ax, TEMPSEG
    mov es, ax
    mov ds, ax

    mov di, 1000h
    mov ax, 4f01h
    mov cx, 115h
    int 10h     ;获取信息，放在es:di中

    mov ax, es:[1000h + SCREEN_WIDTH]
    mov ds:[SCREEN_WIDTH_OFFSET], ax

    mov ax, es:[1000h + SCREEN_HEIGHT]
    mov ds:[SCREEN_HEIGHT_OFFSET], ax

    mov eax, es:[1000h + SCREEN_RAM]
    mov ds:[SCREEN_RAM_OFFSET], eax

    mov ax, 4f02h
    mov bx, 4115h
    int 10h     ;设置显卡信息

getMemoryInfo:
    xor ebx, ebx
    mov edi, MEM_LAYOUT_NUM_OFFSET
    mov dword ptr ds:[MEM_RANGE_NUM_OFFSET], 0
gmi1:
    mov eax, 0e820h
    mov ecx, 20
    mov edx, 534d4150h
    int 15h
    jc gmifailed

    add edi, 20
    inc dword ptr ds:[MEM_RANGE_NUM_OFFSET]
    cmp ebx, 0
    jne gmi1
    jmp gmisuccess

gmifailed:
    mov dword ptr ds:[MEM_RANGE_NUM_OFFSET], 0
gmisuccess:
    cli

    mov ax, BOOTSEG
    mov ds, ax
    
    xor ebx, ebx

    mov eax, BOOTBASE
    add eax, offset gdt
    mov ds:[GdtAddr], eax

    lidt fword ptr ds:[IdtPtr]
    lgdt fword ptr ds:[GdtPtr]   ;希望bootmgr在64K以内以至于ds可以不用发生变化

    mov ax, SETUPSEG    ;获得入口点
    mov es, ax
    mov si, 0
    call getEntryOfPE

    mov ds:[setupoffset], eax
    
    
    in al,92h
	or al,00000010b
	out 92h,al
   
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    mov eax, KERNEL_DATA
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov esp, BOOTBASE - 4

longjmp             db 066h,0eah
setupoffset         dd 0
setupseg            dw KERNEL_CODE

    jmp $

getEntryOfPE proc near ;es:si指向pe的位置，返回eax为入口点
    push ebx
    movzx ebx, si
    cmp word ptr es:[ebx], 05a4dh ;dos头
    jnz getEntryFailed
    add ebx, es:[ebx + 03ch]  
    cmp dword ptr es:[ebx], 04550h   ;PE头
    jnz getEntryFailed
    mov eax, es:[ebx + 028h]   ;入口点
    add eax, es:[ebx + 034h]   ;加上基址
    pop ebx
    ret
getEntryFailed:
    mov cx, 5
    call showerror
    jmp $

getEntryOfPE endp

gdt:
DUMMY   			dq 0
CODE_SELECTOR       dq 00cf9a000000ffffh     ;4K 32 C  DPL0 0-4G
DATA_SELECTOR       dq 00cf92000000ffffh     ;4K 32 RW DPL0 0-4G

GdtPtr					dw  $ - gdt
GdtAddr					dd  0	

IdtPtr                  dw  0
                        dd  0

.endcode16 
end