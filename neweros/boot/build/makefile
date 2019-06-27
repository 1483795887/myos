NATIVE2BIN = $(osbase)\tools\native2bin.exe

asobjs = $(OBJ)\boot.obj #..\$(targ)\loader.asm

TARGET = $(toptarget)\boot.sys# $(osbase)\build\loader.sys

!include $(osbase)\include\mk\makefile

$(toptarget)\boot.sys:$(OBJ)\boot.obj 
	$(NATIVE2BIN) $(OBJ)\boot.obj $(toptarget)\boot.sys 0x7c00
	
