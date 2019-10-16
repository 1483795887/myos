#include <global/OS.h>
#include <arch/CPU.h>
#include <interrupt/Interrrupt.h>
#include <lib/CString.h>

void _cdecl commonIntHandler(PBYTE par) {
    CString str("theInterruptIsWorking");
    os->console->print(str.getBuf());
    while (1);
}



void initSyscall(PBYTE table) {
}

DWORD DefaultInterruptHandler(TrapFrame * frame) {
    return 0;
}

void initInterrupt() {
    PBYTE table = os->allocator->allocPages(0);

    initTrap(table);
    initIOInterrupt(table);
    initSyscall(table);

    CPU::setInterruptVectorTable(table);
}