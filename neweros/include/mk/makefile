
incs = /I. /I$(osbase)\include\ /I$(osbase)\include\asm\ /I$(osbase)\include\fs\ 

machine = x86
targaopts = /coff /Cp 

aopts	  = $(targaopts) 

AS = ml.exe /nologo 
AFLAGS = $(aopts) $(incs) /c /Foobj$(targ)\ 

CC = cl.exe /nologo
CFLAGS = $(incs) /X /Gz /Zl /c /Foobj$(targ)\ 

LINK = link.exe /nologo
targopts = /subsystem:native /driver

LINKFLAGS = $(targopts) /MAP $(particularopts)

!if defined(Debug)
$(LINKFLAGS) = $(LINKFLAGS) /DEBUG
$(CFLAGS) = $(CFLAGS) /Zi
!endif

OBJ = obj$(targ)

build:$(target)

{..\$(targ)\}.asm{$(OBJ)\}.obj::
    $(AS) $(AFLAGS) $<

{..\$(targ)\}.c{$(OBJ)\}.obj::
    $(CC) $(CFLAGS) $<

clean:
	-del /Q $(asobjs) $(ccarchobjs) $(target) $(special)