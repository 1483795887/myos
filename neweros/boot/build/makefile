NATIVE2BIN = $(osbase)\tools\native2bin.exe  

asobjs = $(OBJ)\bootmgr.obj

target = $(toptarget)\bootmgr 

!include $(osbase)\include\mk\makefile

$(toptarget)\bootmgr:$(OBJ)\bootmgr.obj
    $(NATIVE2BIN) $(OBJ)\bootmgr.obj $(toptarget)\bootmgr