#include <arch/CPU.h>
#include <mm/physicalPage.h>

extern "C" void _cdecl doOpenPageMode();
extern "C" void _cdecl doSetGlobalDescriptorTable(PBYTE table);
extern "C" void _cdecl doSetInterruptVectorTable(PBYTE table);

void CPU::openPageMode() {
    doOpenPageMode();
}

void CPU::setGlobalDescriptorTable(PBYTE table) {
    doSetGlobalDescriptorTable(table);
}

void CPU::setInterruptVectorTable(PBYTE table) {
    doSetInterruptVectorTable(table);
}


