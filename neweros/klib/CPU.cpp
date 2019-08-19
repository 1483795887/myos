#include <klib/CPU.h>

extern "C" void _cdecl doOpenPageMode();

void CPU::openPageMode() {
	doOpenPageMode();
}
