#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>

class OS {
public:
    Pool* pool;

    Status getLastStatus();
    void setLastStatus(Status status);

    OS() {
        lastStatus = Success;
        pool = NULL;
    }

private:
	Status lastStatus;
};

extern OS* os;

#define New new(os->pool)

inline void* _cdecl operator new (SIZE size, Pool* pool) {
    if (pool == NULL) {
		os->setLastStatus(NullPointer);
        return NULL;
    }
    return pool->allocate(size);
}

inline void* _cdecl operator new[](SIZE size, Pool* pool) {
    if (pool == NULL) {
		os->setLastStatus(NullPointer);
        return NULL;
    }

    return pool->allocate(size);
}