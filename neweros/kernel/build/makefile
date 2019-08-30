ccobjs = $(OBJ)\main.obj $(OBJ)\Interrupt.obj
ccarchobjs = $(OBJ)\trap.obj
asobjs = $(OBJ)\InterruptA.obj

lib = $(toptarget)\klib.lib $(toptarget)\mm.lib $(toptarget)\lib.lib $(toptarget)\global.lib $(toptarget)\graphic.lib

executable = $(toptarget)\kernel.exe
target = $(executable)

particularopts = /base:0xc0100000 /fixed /align:0x1000 /entry:main

!include $(osbase)\include\mk\makefile