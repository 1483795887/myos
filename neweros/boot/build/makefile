NATIVE2BIN = $(osbase)\tools\native2bin.exe  
#测试一下

asobjs = $(OBJ)\bootmgr.obj
ccarchobjs = $(OBJ)\bootmgrpe.obj

target = $(toptarget)\bootmgr 

particularopts = /section:.text,ERW /merge:.rdata=.text /base:0x10000 /fixed /align:0x400 /merge:.data=.text /entry:start

#special = $(OBJ)\bootmgr.bin $(OBJ)\bootmgrpe.exe

!include $(osbase)\include\mk\makefile

#$(toptarget)\bootmgr:$(special)
#    copy /b $(OBJ)\bootmgr.bin+$(OBJ)\bootmgrpe.sys >> $@



#$(OBJ)\bootmgr.bin:$(OBJ)\bootmgr.obj 
#	$(NATIVE2BIN) $(OBJ)\bootmgr.obj $(toptarget)\bootmgr.bin
    
#$(toptarget)\bootmgrpe.exe:$(OBJ)\bootmgrpe.obj
#    $(LINK) $(LINKFLAGS) $(particularopts) /out:$@ $**


$(toptarget)\bootmgr:$(OBJ)\bootmgr.obj
    $(NATIVE2BIN) $(OBJ)\bootmgr.obj $(toptarget)\bootmgr