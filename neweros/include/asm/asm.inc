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

.code32 macro
    .text segment
endm

.endcode32 macro
	.text ends
endm