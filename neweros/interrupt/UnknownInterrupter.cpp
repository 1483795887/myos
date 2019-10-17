#include "UnknownInterrupter.h"
#include <global/OS.h>
#include <lib/CString.h>
#include <arch/CPU.h>

DWORD UnknownInterrupter::handle(TrapFrame * trap) {
	CString str;
	str.format("unknown interrupt :%d", trap->type);
	os->console->print(str.getBuf());
	halt();
    return 0;
}
