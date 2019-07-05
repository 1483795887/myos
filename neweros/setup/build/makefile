
ccobjs = $(OBJ)\main.obj

target = $(toptarget)\setup.exe

particularopts = /base:0x30000 /fixed /align:0x400 /entry:main

!include $(osbase)\include\mk\makefile

$(toptarget)\setup.exe:$(asobjs) $(ccobjs) $(ccarchobjs)
    $(LINK) $(LINKFLAGS) /out:$@ $**