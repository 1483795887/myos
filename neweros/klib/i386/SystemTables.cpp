#include <arch/KernelTables.h>
#include <arch/i386/Descritpor.h>

void GlobalDescriptorTable::setDescriptor(ULONG n, PBYTE start, ULONG limit, DescriptorType type, Privilege privilege) {
    Descriptor* desTable = (Descriptor*)table;
	limit = limit >> 12;	//除以4K
    ULONG flags = DA_32 | DA_LIMIT_4K;
    switch (privilege) {
    case User:
        flags |= DA_DPL3;
        break;
    case Kernel:
        flags |= DA_DPL0;
        break;
    default:
        flags |= DA_DPL0;
        break;
    }
    switch (type) {
    case Data:
        flags |= DA_DRW;
        break;
    case Code:
        flags |= DA_CR;
        break;
	case TSS:
		flags |= DA_386TSS;
		break;
	default:
		flags |= DA_DRW;
		break;
    }
	desTable[n] = Descriptor((ULONG)start, limit, flags);
}

void InterruptVectorTable::setInterruptVector(IntType vec, InterruptRoutine routine, InterruptType type, Privilege privilege) {

}



Descriptor::Descriptor(ULONG start, ULONG limit, ULONG flags) {
    limitLow = limit & 0xffff;
    baseLow = start & 0xffff;
    baseMid = (start >> 16) & 0xff;
    attr1 = flags & 0xff;
    limitHighAttr2 = ((limit >> 16) & 0x0f) | ((flags >> 8) & 0xf0);
    baseHigh = (start >> 24) & 0xff;
}