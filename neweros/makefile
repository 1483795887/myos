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

modules = boot
buildtargets = $(modules: =.build).build
cleantargets = $(modules: =.clean).clean

default:$(buildtargets) install

$(buildtargets):
	cd $(MAKEDIR)\$(@R)\build
		$(MAKE) $(subargs) module=$(@R)
	cd $(MAKEDIR)
	
    
install:
    mount

    -del /s /q $(masterdisk):\*.*
    -rd /s /q $(masterdisk):\root 
    mkdir $(masterdisk):\root
    copy /b $(toptarget)\bootmgr $(masterdisk):\root\bootmgr

    unmount

clean:$(cleantargets) remove

$(cleantargets):
	cd $(MAKEDIR)\$(@R)\build
		@$(MAKE) $(subargs) module=$(@R) clean
	@cd $(MAKEDIR)

remove:
	echo "remove"