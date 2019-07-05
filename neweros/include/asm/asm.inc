.386p
.model flat
ASSUME CS:NOTHING, DS:NOTHING, ES:NOTHING, FS:NOTHING, GS:NOTHING
option dotname

.code16 macro
	.text segment use16
endm

.endcode16 macro
	.text ends
endm

.jmp232 macro seg,offset
    db 
endm