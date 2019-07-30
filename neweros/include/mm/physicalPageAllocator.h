#pragma once
#include "../types.h"
#include "zone.h"

class PhysicalPageAllocator {
public:
    virtual ULONG allocPages(Zone* zone, ULONG order) = 0;
	virtual ~PhysicalPageAllocator() {

	}
};