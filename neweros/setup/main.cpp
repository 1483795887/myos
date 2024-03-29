#include <types.h>
#include <fs/pe.h>
#include <lib/Memory.h>
#include <mm/PageMapper.h>
#include <graphic/Graphic.h>
#include <arch/CPU.h>
#include <global/BootParam.h>
#include <global/OS.h>
#include <mm/PageAllocator.h>
#include "setup.h"

class NaiveAllocator : public PageAllocator {
public:
    NaiveAllocator(ULONG* kernelStart) {
        this->kernelStart = kernelStart;
    }

    virtual PBYTE allocPages(ULONG order) {
        //虽然这里只用到order= 0 但还是写
        SIZE size = getPageSizeByOrder(order);
        ULONG addr = *kernelStart;
        *kernelStart += size;
        return (PBYTE)addr;
    };

private:
    ULONG *kernelStart;
};

ULONG kernelStart = 0; //物理地址

ULONG codeStart, codeSize, dataStart, dataSize, rdataStart, rdataSize;//虚拟地址
ULONG entryPoint;

ULONG memoryEnd;    //物理地址
PD pd;

PBYTE pool;
SIZE poolSize;

BootParams bootParams;

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

    ULONG kernelBase = getPAFromVA(KernelImageBase);

    memcpy((PBYTE)kernelBase, (PBYTE)KernelTempBase,  sections[0].rawAddress);

    for (int i = 0; i < numberOfSections; i++) {
        if (sections->virtualAddress + sections->virtualSize + kernelBase > kernelStart)
            kernelStart = sections->virtualAddress + sections->virtualSize + kernelBase;
        if (sections->characteristics & SECTION_FLAG_CONTAIN_CODE) {
            codeStart = sections->virtualAddress + KernelImageBase;
            codeSize = sections->virtualSize;
        }
        if (sections->characteristics & SECTION_FLAG_CONTAIN_INITAILIZED_DATA) {
            if (sections->characteristics & SECTION_FLAG_WRITABLE) {
                dataStart = sections->virtualAddress + KernelImageBase;
                dataSize = sections->virtualSize;
            } else {
                rdataStart = sections->virtualAddress + KernelImageBase;
                rdataSize = sections->virtualSize;

            }
        }
        memcpy(
            (PBYTE)(sections->virtualAddress + getPAFromVA(KernelImageBase)),
            (PBYTE)(KernelTempBase + sections->rawAddress),
            sections->rawSize);
        sections++;
    }
    entryPoint = optionalHeader->addressOfEntryPoint + KernelImageBase;

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
    memoryEnd = usableMemoryLength;
    kernelStart = ulAlign(kernelStart, PAGE_SIZE, TRUE);

    pool = (PBYTE)(kernelStart + KERNEL_BASE);
    poolSize = getPoolSize(usableMemoryLength);

    kernelStart += poolSize;
    dataSize += poolSize;
}

void initMemory() {

    NaiveAllocator allocator(&kernelStart);

    pd = (PD)allocator.allocPages(0);

    PageMapper::mapPages(pd, 0, 0, KernelImageBase - KERNEL_BASE, PMWritable | PMSupervisor | PMExistent,
                         &allocator);
    PageMapper::mapPages(pd, getPAFromVA(codeStart), codeStart, codeSize, PMSupervisor | PMExistent,
                         &allocator);
    PageMapper::mapPages(pd, getPAFromVA(rdataStart), rdataStart, rdataSize, PMSupervisor | PMExistent,
                         &allocator);

    PageMapper::mapPages(pd, getPAFromVA(dataStart), dataStart,
                         memoryEnd - getPAFromVA(dataStart), PMWritable | PMSupervisor | PMExistent,
                         &allocator);    //dataSize包括了初始池

    ULONG* graphicInfos = (ULONG*)GraphicInfo;
    ULONG width = graphicInfos[0] & 0xffff;
    ULONG height = graphicInfos[0] >> 16;
    ULONG vam = (ULONG)graphicInfos[1];

    PageMapper::mapPages(pd, vam, vam, width * height * sizeof(RGB), PMWritable | PMSupervisor | PMExistent,
                         &allocator);

    bootParams.graphicInfo = (PBYTE)GraphicInfo;
    bootParams.font = (PBYTE)DotFontMap;

    PageMapper::changePD(pd);
    openPageMode();
}

typedef void(*KernelEntry)(BootParams* bootParams);

void main() {

    copyKernel();
    getMemorySize();
    initMemory();

    bootParams.codeSize = codeSize;
    bootParams.codeStart = codeStart;
    bootParams.dataSize = dataSize;
    bootParams.dataStart = dataStart;
    bootParams.rdataSize = rdataSize;
    bootParams.rdataStart = rdataStart;
    bootParams.kernelStart = kernelStart;
    bootParams.memoryEnd = memoryEnd;
    bootParams.pd = pd;
    bootParams.pool = pool;
    bootParams.poolSize = poolSize;

    KernelEntry kernelEntry = (KernelEntry)entryPoint;
    kernelEntry(&bootParams);

    for (;;);
}