
incs = /I. /I$(osbase)\include\ /I$(osbase)\include\asm\ /I$(osbase)\include\fs\ 

machine = x86
targaopts = /coff /Cp 

aopts	  = $(targaopts) 

AS = ml.exe /nologo 
AFLAGS = $(aopts) $(incs) /c /Foobj$(targ)\ 

CC = cl.exe /nologo
CFLAGS = $(incs) /X /Gz /Zl /c /utf-8 /GR- /Foobj$(targ)\ 

# /X忽略包含标准目录 /Gz默认使用stdcall /Zl从.obj中移除默认库名

LINK = link.exe /nologo
targopts = /subsystem:native /driver /nodefaultlib

LINKFLAGS = $(targopts) /MAP $(particularopts)

LIB		  = lib.exe -nologo

!if defined(Debug)
LINKFLAGS = $(LINKFLAGS) /DEBUG
debugData = $(toptarget)\$(module).pdb
CFLAGS = $(CFLAGS) /Zi /DEBUG /Fd$(debugData)
!endif

OBJ = obj$(targ)

build:$(target)

{..\$(targ)\}.asm{$(OBJ)\}.obj::
    $(AS) $(AFLAGS) $<

{..\}.cpp{$(OBJ)\}.obj::
    $(CC) $(CFLAGS) $<

{..\$(targ)\}.cpp{$(OBJ)\}.obj::
    $(CC) $(CFLAGS) $<

!if defined(library)
$(library): $(asobjs) $(ccarchobjs) $(ccobjs)
	$(LIB) -out:$@ $**
!endif

!if defined(executable)
mapfile = $(executable:.exe=.map)
$(executable): $(asobjs) $(ccarchobjs) $(ccobjs) $(lib)
    $(LINK) $(LINKFLAGS) /out:$@ $**
!endif

clean:
	-del /Q $(asobjs) $(ccarchobjs) $(ccobjs) $(target) $(debugData) $(mapfile)
