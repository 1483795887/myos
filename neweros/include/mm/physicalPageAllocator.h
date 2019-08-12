#pragma once
#include "../Types.h"
#include "Zone.h"

class PhysicalPageAllocator {
public:
	virtual PBYTE allocPages(Zone* zone, ULONG order) {
		return NULL;
	};
	virtual ~PhysicalPageAllocator() {
	};
};