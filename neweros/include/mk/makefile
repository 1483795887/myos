
incs = /I. /I$(osbase)\include\ /I$(osbase)\include\asm\ 

machine = x86
targaopts = /coff /Cp 

aopts	  = $(targaopts) 

AS = ml.exe /nologo 
AFLAGS = $(aopts) $(incs) /c /Foobj$(targ)\ 

CC = cl.exe /nologo
CFLAGS = $(incs) /c /Foobj$(targ)\ 

OBJ = obj$(targ)

build:$(TARGET)

{..\$(targ)\}.asm{$(OBJ)\}.obj::
	echo $<
	$(AS) $(AFLAGS) $<

clean:
	-del /Q $(asobjs) $(TARGET)