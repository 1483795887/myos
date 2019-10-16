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

    Graphic* graphic;
    Console* console;

    Status getLastStatus();
    void setLastStatus(Status status);

    PageAllocator* allocator;
    PageMapper* pageMapper;

    OS() {
        lastStatus = StatusSuccess;
        pool = NULL;
    }

private:
    Status lastStatus;

};

extern OS* os;

void* _cdecl operator new (SIZE size, Pool* pool);
void* _cdecl operator new[](SIZE size, Pool* pool);


#define New new(os->pool)