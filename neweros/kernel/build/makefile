
ccobjs = $(OBJ)\main.obj

target = $(toptarget)\kernel.exe

particularopts = /base:0xc0000000 /fixed /align:0x1000 /entry:main

!include $(osbase)\include\mk\makefile

$(toptarget)\kernel.exe:$(asobjs) $(ccobjs) $(ccarchobjs)
    $(LINK) $(LINKFLAGS) /out:$@ $**