NATIVE2BIN = $(osbase)\tools\native2bin.exe  
#测试一下

asobjs = $(OBJ)\mbr.obj $(OBJ)\boot.obj #..\$(targ)\loader.asm

TARGET = $(toptarget)\mbr.sys $(toptarget)\boot.sys# $(osbase)\build\loader.sys

!include $(osbase)\include\mk\makefile

$(toptarget)\mbr.sys:$(OBJ)\mbr.OBJ
    $(NATIVE2BIN) $(OBJ)\mbr.obj $(toptarget)\mbr.sys

$(toptarget)\boot.sys:$(OBJ)\boot.obj 
	$(NATIVE2BIN) $(OBJ)\boot.obj $(toptarget)\boot.sys