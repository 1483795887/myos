#pragma once

#include <Types.h>
#include <Status.h>
#include <mm/Pool.h>

class OS {
public:
	Status lastStatus;
	Pool* pool;

	OS() {
		lastStatus = Success;
		pool = NULL;
	}
};

extern OS* os;

#define New new(os->pool)

inline void* _cdecl operator new(SIZE size, Pool* pool) {
	if (pool == NULL) {
		os->lastStatus = NullPointer;
		return NULL;
	}
	return pool->allocate(size);
}

inline void* _cdecl operator new[](SIZE size, Pool* pool) {
	if (pool == NULL) {
		os->lastStatus = NullPointer;
		return NULL;
	}
		
	return pool->allocate(size);
}