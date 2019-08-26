#pragma once

#include "PhysicalPageAllocator.h"

class PhysicalPageAllocatorImpl : public PhysicalPageAllocator {
public:
	PhysicalPageAllocatorImpl();

    virtual PBYTE allocPages(ULONG order);
    virtual void putPage(PBYTE page);
    void init(PBYTE start, SIZE size);
	void startVirtualMemory();
private:
	BOOL virtualMemory;
    Zone* zone;
    Page* memMap;
};