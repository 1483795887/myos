#pragma once
#include "../types.h"
#include <lib/CList.h>
#include <mm/MemoryAllocator.h>

#define MAX_ORDER 10

class FreeArea {
public:
	CList freeList;
	PBYTE map;
};

class Zone {
public:
	void initFreeAreas(PBYTE start, ULONG memorySize);
private:
	PBYTE start;
	ULONG end;
	FreeArea freeLists[MAX_ORDER];
};

class ZoneFactory {
public:
	ZoneFactory(MemoryAllocator* allocator);
	Zone* makeZone(PBYTE start, ULONG memorySize);
private:
	MemoryAllocator* allocator;
};