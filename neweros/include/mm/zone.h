#pragma once
#include "../types.h"
#include <lib/CList.h>

#define MAX_ORDER 10

class FreeArea {
public:
	CList freeList;
	PBYTE map;
};

class Zone {
public:
private:
	ULONG start;
	ULONG size;
};