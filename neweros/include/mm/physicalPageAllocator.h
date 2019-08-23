#pragma once
#include "../Types.h"
#include "Zone.h"

class PhysicalPageAllocator {
public:
	virtual PBYTE allocPages(ULONG order) {
		return NULL;
	};
	virtual void putPage(PBYTE page) {

	}
	virtual ~PhysicalPageAllocator() {
	};
};