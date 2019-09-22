#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>
#include <mm/PageMapper.h>
#include <graphic/Graphic.h>
#include <graphic/Console.h>



class OS {
public:
    Pool* pool;

    //ULONG start;        //物理地址
    //ULONG end;

    //ULONG codeStart;    //虚拟地址
    /*SIZE codeSize;
    ULONG dataStart;
    SIZE dataSize;
    ULONG rdataStart;
    SIZE rdataSize;

    ULONG entryPoint;*/

    Graphic* graphic;
    Console* console;

    Status getLastStatus();
    void setLastStatus(Status status);

    PhysicalPageAllocator* allocator;
    PageMapper* pageMapper;

    OS() {
        lastStatus = StatusSuccess;
        pool = NULL;
    }

	void run();

private:
    Status lastStatus;

};

extern OS* os;

void* _cdecl operator new (SIZE size, Pool* pool);
void* _cdecl operator new[](SIZE size, Pool* pool);


#define New new(os->pool)