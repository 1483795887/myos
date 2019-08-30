#include <arch/KernelTables.h>
#include <arch/i386/Descritpor.h>
#include <interrupt/i386/i386Interrupt.h>
#include <arch/CPU.h>

void GlobalDescriptorTable::setDescriptor(ULONG n, PBYTE start, ULONG limit, DescriptorType type, Privilege privilege) {
    Descriptor* desTable = (Descriptor*)table;
    limit = limit >> 12;    //除以4K
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

void InterruptVectorTable::setTrapHandler(int no, InterruptRoutine handler) {
    Gate* gateTable = (Gate*)table;
	BYTE flags = 0;
	if (no != BreakPointVec)
		flags = (DA_386TGate | DA_DPL0);
	else
		flags = (DA_386TGate | DA_DPL3);
    Selector selector = KernelCodeSel;
    gateTable[no] = Gate((ULONG)handler, flags, selector);
}

void InterruptVectorTable::setInterruptHandler(int no, InterruptRoutine handler) {
    Gate* gateTable = (Gate*)table;
    BYTE flags = (DA_386IGate | DA_DPL0);
    Selector selector = KernelCodeSel;
    gateTable[no] = Gate((ULONG)handler, flags, selector);
}

void InterruptVectorTable::setSyscallHandler(InterruptRoutine handler) {
    Gate* gateTable = (Gate*)table;
    BYTE flags = (DA_386IGate | DA_DPL3);
    Selector selector = KernelCodeSel;
    gateTable[getArchIntNoFromIntType(Syscall)] = Gate((ULONG)handler, flags, selector);
}

InterruptVectorTable::InterruptVectorTable(PBYTE table) {
    this->table = table;
    CPU::setInterruptVectorTable(table);
}


int InterruptVectorTable::getArchIntNoFromIntType(IntType type) {
	int no = 0;
	switch (type) {
	case DivideZero:
		no = DivideZeroVec;
		break;
	}
    return 0;
}

Descriptor::Descriptor(ULONG start, ULONG limit, ULONG flags) {
    limitLow = limit & 0xffff;
    baseLow = start & 0xffff;
    baseMid = (start >> 16) & 0xff;
    attr1 = flags & 0xff;
    limitHighAttr2 = ((limit >> 16) & 0x0f) | ((flags >> 8) & 0xf0);
    baseHigh = (start >> 24) & 0xff;
}

Gate::Gate(ULONG handlerAddress, BYTE flags, Selector selector) {
    handlerAddressLow = handlerAddress & 0xffff;
    handlerAddressHigh = (handlerAddress >> 16) & 0xffff;
    this->selector = selector;
    this->flags = flags;
	count = 0;
}