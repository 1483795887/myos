#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>
#include <mm/PhysicalPage.h>
#include <graphic/Graphic.h>


class OS {
public:
    Pool* pool;

    ULONG start;
    ULONG end; 

	ULONG codeStart;
	SIZE codeSize;
	ULONG dataStart;
	SIZE dataSize;

	ULONG entryPoint;

	Graphic* graphic;

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