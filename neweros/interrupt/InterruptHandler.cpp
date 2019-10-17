#include <global/OS.h>
#include <arch/CPU.h>
#include <interrupt/Interrrupt.h>
#include <lib/CString.h>
#include "UnknownInterrupter.h"

void _cdecl commonIntHandler(TrapFrame* frame) {
	os->irqs[frame->type].handle(frame);
}



void initSyscall(PBYTE table) {
}

DWORD DefaultInterruptHandler(TrapFrame * frame) {
    return 0;
}

void initInterrupt() {

	os->irqs = New IRQ[MAX_IRQ];
	Interrupter* unknownInterrupter = New UnknownInterrupter;

	for (int i = 0; i < MAX_IRQ; i++) {
		os->irqs->setIRQ(i);
		os->irqs->bindInterrupter(unknownInterrupter);
	}

    PBYTE table = os->allocator->allocPages(0);

    initTrap(table);
    initIOInterrupt(table);
    initSyscall(table);

    setInterruptVectorTable(table);
}