subargs = /$(MAKEFLAGS) osbase=$(MAKEDIR)

masterdisk = O
toptarget = $(MAKEDIR)\build\target

!if defined(x86)
subargs		= $(subargs) targ=i386 topobj=$(MAKEDIR)\build\obji386 toptarget=$(toptarget) 
targ		= i386
lmachine	= x86
!else
!ERROR Usage: make (x86) [clean]
!endif

#测试一下

modules=boot setup kernel
buildtargets = $(modules: =.build ).build
cleantargets = $(modules: =.clean ).clean

default:$(buildtargets) install

$(buildtargets):
	cd $(MAKEDIR)\$(@R)\build
		$(MAKE) $(subargs) module=$(@R)
	cd $(MAKEDIR)
	
    
install:
    mount

    -mkdir $(masterdisk):\root
    copy /b /y $(toptarget)\bootmgr $(masterdisk):\bootmgr
    copy /b /y $(toptarget)\setup.exe $(masterdisk):\root\setup.exe
    copy /b /y $(toptarget)\kernel.exe $(masterdisk):\root\kernel.exe

    unmount

clean:$(cleantargets) remove

$(cleantargets):
	cd $(MAKEDIR)\$(@R)\build
		@$(MAKE) $(subargs) module=$(@R) clean
	@cd $(MAKEDIR)

remove:
	echo "remove"