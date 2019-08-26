#include <types.h>
#include <fs/pe.h>
#include <lib/Memory.h>
#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/NaivePool.h>
#include <mm/PhysicalPage.h>
#include <mm/PhysicalPageAllocatorImpl.h>
#include <graphic/DotFont.h>
#include <klib/CPU.h>
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

    ULONG kernelBase = getPAFromVA(KernelImageBase);

    memcpy((PBYTE)kernelBase, (PBYTE)KernelTempBase,  sections[0].rawAddress);

    ULONG kernelStart = 0; //物理地址

    ULONG codeStart, codeSize, dataStart, dataSize, rdataStart, rdataSize;//虚拟地址

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

    NaivePool oriPool(0, 0);
    NaivePool* pool = (NaivePool*)kernelStart;
    memcpy((PBYTE)pool, (PBYTE)&oriPool, sizeof(NaivePool));//要虚函数表，但pool没有，所以复制局部变量的，反正都一样
    pool->setStart((PBYTE)pool + sizeof(NaivePool));
    pool->setSize(sizeof(OS)); //暂时存os之后再扩展
    os = (OS*)pool->allocate(sizeof(OS));
    os->setLastStatus(Success);
    os->pool = pool;

    os->codeStart = codeStart;
    os->codeSize = codeSize;
    os->dataStart = dataStart;
    os->dataSize = dataSize;
    os->rdataStart = rdataStart;
    os->rdataSize = rdataSize;

    os->entryPoint = optionalHeader->addressOfEntryPoint + KernelImageBase;

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
    os->end = usableMemoryLength;
    SIZE poolSize = getPoolSize(usableMemoryLength);
    os->start = ulAlign((ULONG)os->pool + poolSize, getPageSizeByOrder(MAX_ORDER), TRUE);
    ((NaivePool*)os->pool)->setSize(poolSize);
}

void initGraphic() {
    Graphic* graphic = New Graphic();
    Rect rect;
    WORD* graphicInfos = (WORD*)GraphicInfo;
    rect.width = graphicInfos[0];
    rect.height = graphicInfos[1];
    graphic->init(&rect, *(PBYTE*)&graphicInfos[2]);
    os->graphic = graphic;

    DotFont* font = New DotFont((PBYTE)DotFontMap, 8, 16);
    font->setColor(WHITE);

    Console* console = New Console(&rect);
    console->setFont(font);
    os->console = console;

}

void initMemory() {
    PhysicalPageAllocatorImpl* allocator = New PhysicalPageAllocatorImpl;
    allocator->init((PBYTE)os->start, os->end - os->start);
    os->allocator = allocator;

    PhysicalPageManager* ppm = New PhysicalPageManager;
    ppm->setAllocator(allocator);
    ppm->init();
    //ppm->init((PBYTE)os->start, os->end - os->start);
    os->ppm = ppm;

    ppm->mapPages(0, 0,
                  os->start,
                  Writable | Supervisor | Existence);
    ppm->mapPages(getPAFromVA(os->codeStart),
                  os->codeStart,
                  os->codeSize,
                  Supervisor | Existence);
    ppm->mapPages(getPAFromVA(os->rdataStart),
                  os->rdataStart,
                  os->rdataSize,
                  Supervisor | Existence);
    ppm->mapPages(getPAFromVA(os->dataStart),
                  os->dataStart,
                  os->end - getPAFromVA(os->dataStart),
                  Writable | Supervisor | Existence);

    ULONG* graphicInfos = (ULONG*)GraphicInfo;
    ULONG width = graphicInfos[0] & 0xffff;
    ULONG height = graphicInfos[0] >> 16;
    ULONG vam = (ULONG)graphicInfos[1];

    ppm->mapPages((ULONG)vam,
                  (ULONG)vam,
                  width * height * sizeof(RGB),
                  Writable | Supervisor | Existence);

    ppm->changePD();
    CPU::openPageMode();
}

typedef void(*KernelEntry)(OS*);

void main() {

    copyKernel();
    getMemorySize();
    initGraphic();
    initMemory();

    KernelEntry kernelEntry = (KernelEntry)os->entryPoint;
    kernelEntry(os);

    for (;;);
}