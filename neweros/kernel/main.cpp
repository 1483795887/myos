#include <global/OS.h>
#include <global/BootParam.h>
#include <mm/NaivePool.h>
#include <mm/BucketPool.h>
#include <mm/PhysicalPageAllocator.h>
#include <graphic/Graphic.h>
#include <graphic/DotFont.h>
#include <lib/CString.h>
#include <lib/Memory.h>
#include "Kernel.h"

/*void initGdt() {
    PBYTE globalDescriptorTable = os->pageMapper->allocatePage(0);

    kernelGlobal->gdt = New GlobalDescriptorTable(globalDescriptorTable);

    kernelGlobal->gdt->setDescriptor(KernelCode, 0, 4 * G - 1, Code, Kernel);
    kernelGlobal->gdt->setDescriptor(KernelData, 0, 4 * G - 1, Data, Kernel);
    kernelGlobal->gdt->setDescriptor(UserCode, 0, 4 * G - 1, Code, User);
    kernelGlobal->gdt->setDescriptor(UserData, 0, 4 * G - 1, Data, User);

    CPU::setGlobalDescriptorTable(globalDescriptorTable);
}
*/

void initOS(BootParams* bootParams) {
    NaivePool* pool = (NaivePool*)bootParams->pool;
    PBYTE poolAddr = bootParams->pool + sizeof(NaivePool);
    NaivePool tempPool = NaivePool(poolAddr, bootParams->poolSize);
    memcpy(pool, &tempPool, sizeof(NaivePool)); //pool没有虚函数表，得给它补上

    os = (OS*)poolAddr;
    pool->setStart(poolAddr + sizeof(OS));  //os不能使用new因为new要通过它
    pool->setSize(bootParams->poolSize - sizeof(OS));   //不减也行，因为留了4个页
    os->pool = pool;
}

void initMem(BootParams* bootParams) {
    PhysicalPageAllocator* allocator = New PhysicalPageAllocator;
    allocator->init((PBYTE)bootParams->kernelStart,
                    bootParams->memoryEnd - bootParams->kernelStart);
    os->allocator = allocator;

	BucketPool* pool = New BucketPool;
	pool->setAllocator(allocator);
	os->pool = pool;
}

void initGraphic(BootParams* bootParams) {
    Graphic* graphic = New Graphic();
    Rect rect;
    WORD* graphicInfos = (WORD*)bootParams->graphicInfo;
    rect.width = graphicInfos[0];
    rect.height = graphicInfos[1];
    graphic->init(&rect, *(PBYTE*)&graphicInfos[2]);
    os->graphic = graphic;
    
    DotFont* font = New DotFont(bootParams->font, 8, 16);
    font->setColor(WHITE);

    Console* console = New Console(&rect);
    console->setFont(font);
    os->console = console;

}

void main(BootParams* bootParams) {

    initOS(bootParams);
    initMem(bootParams);
    initGraphic(bootParams);

    CString str;
    for (int i = 0; i < 100; i++) {
        str.format("%d ", i);
        os->console->print(str.getBuf());
    }

    for (;;);
}


void KernelProcess::run() {

}