#pragma once
#include "../Types.h"
#include "Zone.h"

#define NOT_ASSIGNED (PBYTE)0

class PhysicalPageAllocator {
public:
	virtual PBYTE allocPages(ULONG order, PBYTE address) {
		return NULL;
	};
	virtual void putPage(PBYTE page) {

	}
	virtual ~PhysicalPageAllocator() {
	};
};