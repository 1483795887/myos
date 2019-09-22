#include <global/OS.h>
#include <arch/CPU.h>
#include <interrupt/Interrrupt.h>
#include <lib/CString.h>




void _cdecl trapHandler(PBYTE par) {
	CString number;
	number.format("%d", ((ULONG*)par)[0]);
	os->console->print(number.getBuf());

	while (1);
}

void _cdecl interruptHandler(PBYTE par) {

}

void _cdecl syscallHandler(PBYTE par) {

}

void initIOInterrupt(InterruptVectorTable* ivt) {

}

void initSyscall(InterruptVectorTable* ivt) {

}

void initInterrupt() {
	PBYTE table = os->allocator->allocPages(0, NOT_ASSIGNED);
	InterruptVectorTable* ivt = New InterruptVectorTable(table);

	initTrap(ivt);
	initIOInterrupt(ivt);
	initSyscall(ivt);
}