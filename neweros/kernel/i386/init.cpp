#include "../Kernel.h"
#include <global/OS.h>
#include <arch/i386/SystemTables.h>
#include <arch/CPU.h>

void initGlobalDescript() {
	PBYTE table = os->allocator->allocPages(0);

	setDescriptor(table, SegKernelCode, 0, MAX_MEMORY, SegTypeCode, Kernel);
	setDescriptor(table, SegKernelData, 0, MAX_MEMORY, SegTypeData, Kernel);
	setDescriptor(table, SegUserCode, 0, MAX_MEMORY, SegTypeCode, User);
	setDescriptor(table, SegUserData, 0, MAX_MEMORY, SegTypeData, User);

	CPU::setGlobalDescriptorTable(table);
}

void initArch() {
	initGlobalDescript();
}