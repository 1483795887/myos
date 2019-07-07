subargs = /$(MAKEFLAGS) osbase=$(MAKEDIR)

toptarget = $(MAKEDIR)\build\target

!if defined(x86)
subargs		= $(subargs) targ=i386 topobj=$(MAKEDIR)\build\obji386 toptarget=$(toptarget) 
targ		= i386
lmachine	= x86
!else
!ERROR Usage: make (x86) [clean]
!endif

#测试一下

modules=boot klib fs setup kernel
buildtargets = $(modules: =.build ).build
cleantargets = $(modules: =.clean ).clean

default:$(buildtargets)

$(buildtargets):
	cd $(MAKEDIR)\$(@R)\build
		$(MAKE) $(subargs) module=$(@R)
	cd $(MAKEDIR)

clean:$(cleantargets)

$(cleantargets):
	cd $(MAKEDIR)\$(@R)\build
		@$(MAKE) $(subargs) module=$(@R) clean
	@cd $(MAKEDIR)