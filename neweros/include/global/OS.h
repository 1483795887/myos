#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>
#include <mm/PhysicalPage.h>
#include <graphic/Graphic.h>
#include <graphic/Console.h>
#include <arch/KernelTables.h>

class OS {
public:
    Pool* pool;

    ULONG start;        //物理地址
    ULONG end;

    ULONG codeStart;    //虚拟地址
    SIZE codeSize;
    ULONG dataStart;
    SIZE dataSize;
    ULONG rdataStart;
    SIZE rdataSize;

    ULONG entryPoint;

    Graphic* graphic;
    Console* console;

    Status getLastStatus();
    void setLastStatus(Status status);

    PhysicalPageAllocator* allocator;
    PhysicalPageManager* ppm;

    GlobalDescriptorTable gdt;
    InterruptVectorTable ivt;

    OS() {
        lastStatus = Success;
        pool = NULL;
    }

private:
    Status lastStatus;

};

extern OS* os;

void* _cdecl operator new (SIZE size, Pool* pool);
void* _cdecl operator new[](SIZE size, Pool* pool);


#define New new(os->pool)