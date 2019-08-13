#include <types.h>
#include <fs/pe.h>
#include <klib/Memory.h>
#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/NaivePool.h>
#include <mm/PhysicalPage.h>
#include <mm/PhysicalPageAllocatorImpl.h>
#include "setup.h"

void copyKernel() {
    PBYTE address = (PBYTE)KernelTempBase;
    DosHeader* kernelHeader = (DosHeader*)address;

    address += kernelHeader->lfanew;
    NtHeader* ntHeader = (NtHeader*)address;

    address += sizeof(NtHeader);
    OptionalHeader* optionalHeader = (OptionalHeader*)address;

    DWORD imageBase = optionalHeader->imageBase;
    DWORD entryAddress = imageBase + optionalHeader->addressOfEntryPoint;
    DWORD numberOfSections = ntHeader->numberOfSections;

    address += ntHeader->sizeOfOptionalHeader;
    Section* sections = (Section*)address;

	memcpy((PBYTE)KernelTempBase, (PBYTE)getPAFromVA(KernelImageBase), sections[0].rawAddress);

    ULONG kernelStart = 0;

    for (int i = 0; i < numberOfSections; i++) {
        if (sections[i].virtualAddress + sections[i].virtualSize > kernelStart)
            kernelStart = sections[i].virtualAddress + sections[i].virtualSize;
        if (sections[i].characteristics & SECTION_FLAG_CONTAIN_CODE) {      //kernel只有两个段，code与data
            os->codeStart = sections[i].virtualAddress;
            os->codeSize = sections[i].virtualSize;
        }
        if (sections[i].characteristics & SECTION_FLAG_CONTAIN_INITAILIZED_DATA) {
            os->dataStart = sections[i].virtualAddress;
            os->dataSize = sections[i].virtualSize;
        }
		memcpy(
            (PBYTE)(KernelTempBase + sections[i].rawAddress),
            (PBYTE)(getPAFromVA(sections[i].virtualAddress)),
            sections[i].rawSize);
    }

    PBYTE physicalAddress = (PBYTE)getPAFromVA(kernelStart);

    NaivePool* pool = (NaivePool*)physicalAddress;
    pool->setStart((PBYTE)pool + sizeof(NaivePool));
    pool->setSize(sizeof(OS)); //暂时存os之后再扩展
    os = New OS();
    os->pool = pool;

	os->entryPoint = optionalHeader->addressOfEntryPoint + os->codeStart;

}

SIZE getPoolSize(SIZE memorySize) {
	SIZE totalSize = 0;
	SIZE mapSize = memorySize >> (LOG2_PAGE_SIZE + 3);
	SIZE pageArrSize = (memorySize >> (LOG2_PAGE_SIZE)) * sizeof(Page);
	SIZE otherSize = PAGE_SIZE * 4;
	totalSize = mapSize + pageArrSize + otherSize;
	totalSize = ulAlign(totalSize, PAGE_SIZE, TRUE);
	return totalSize;
}

void getMemorySize() {
    int i;
    DWORD usableMemoryStart, usableMemoryLength;
    MemInfo* memInfo = (MemInfo*)MemoryInfo;
    for (i = 0; i < memInfo->rangeNums; i++) {
        if (memInfo->ranges[i].baseAddrLow >= M && memInfo->ranges[i].type == 1) {
            usableMemoryStart = memInfo->ranges[i].baseAddrLow;
            usableMemoryLength = memInfo->ranges[i].lengthLow;
        }
    }
    os->end = KERNEL_BASE + usableMemoryLength;
	SIZE poolSize = getPoolSize(usableMemoryLength);
	os->start = (ULONG)os->pool + poolSize;
	((NaivePool*)os->pool)->setSize(poolSize);
}

typedef void(*KernelEntry)(OS*);

void initMemory() {
	/*Zone* zone = New Zone;
    zone->init((PBYTE)os->start, os->end - os->start);
    zone->putAllPages();

	PhysicalPageAllocator* allocator = New PhysicalPageAllocatorImpl;
    PD pd = (PD)allocator->allocPages(zone, 0);*/
    PhysicalPageManager* ppm = New PhysicalPageManager;
	ppm->init((PBYTE)os->start, os->end - os->start);

    os->setPhysicalPageManager(ppm);

    ppm->mapPages(0, KERNEL_BASE, 
		KernelImageBase - KERNEL_BASE, 
		Writable | Supervisor | Existence);
    ppm->mapPages(getPAFromVA(os->codeStart), 
		os->codeStart, 
		os->codeSize, 
		Supervisor | Existence);
    ppm->mapPages(getPAFromVA(os->dataStart), 
		os->dataStart, 
		os->end - os->dataStart, 
		Writable | Supervisor | Existence);

	KernelEntry kernelEntry = (KernelEntry)os->entryPoint;
	kernelEntry(os);		
}

void main() {

    copyKernel();
    getMemorySize();
	initMemory();

    for (;;);
}