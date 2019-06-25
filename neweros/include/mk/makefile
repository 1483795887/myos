
incs = -I.

machine = x86
targaopts = -coff -Cp

aopts	  = $(targaopts)

AS = ml.exe -nologo
AFLAGS = $(aopts) $(incs) -Foobj$(targ)\ 

CC = cl.exe -nologo
CFLAGS = $(copts) $(incs) -Foobj$(targ)\ 

OBJ = obj$(targ)

#build:$(topobj)\$(library).lib

#clean:
	#-del $(ccobjs) $(asobjs) /Q

#$(topobj)\$(library).lib:$(ccobjs) $(asobjs)
#	@echo linking $(library).lib
#	$(LIB) $(LIBFLAGS) -out:$@ $**

{..\}.cpp{$(OBJ)\}.obj::
	$(CC) $(CFLAGS) -c $<

{..\$(targ)\}.asm{$(OBJ)\}.obj::
	$(AS) $(AFLAGS) -c $<