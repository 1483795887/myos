.386
.model flat
ASSUME CS:NOTHING, DS:NOTHING, ES:NOTHING, FS:NOTHING, GS:NOTHING
option dotname

.code16 macro
	.text segment use16
	assume NOTHING
endm

.endcode16 macro
	.text ends
endm