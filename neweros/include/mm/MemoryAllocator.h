#pragma once
#include "../types.h"

class MemoryAllocator {
public:
	virtual PBYTE allocate(ULONG size);
};