#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>
#include <mm/PhysicalPage.h>
#include <graphic/Graphic.h>
#include <graphic/Console.h>


class OS {
public:
    Pool* pool;

    ULONG start;		//物理地址 
    ULONG end; 

	ULONG codeStart;	//虚拟地址
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

    PhysicalPageManager* getPhysicalPageManager();
    void setPhysicalPageManager(PhysicalPageManager* ppm);

    OS() {
        lastStatus = Success;
        pool = NULL;
    }

private:
    Status lastStatus;
    PhysicalPageManager* ppm;
};

extern OS* os;

#define New new(os->pool)

inline void* operator new (SIZE size, Pool* pool) {
    if (pool == NULL) {
        os->setLastStatus(NullPointer);
        return NULL;
    }
    return pool->allocate(size);
}

inline void* operator new[](SIZE size, Pool* pool) {
    if (pool == NULL) {
        os->setLastStatus(NullPointer);
        return NULL;
    }

    return pool->allocate(size);
}