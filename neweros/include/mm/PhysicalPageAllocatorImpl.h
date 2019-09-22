#pragma once

#include "PhysicalPageAllocator.h"

class PhysicalPageAllocatorImpl : public PhysicalPageAllocator {
public:
    virtual PBYTE allocPages(ULONG order, ULONG address);
    virtual void putPage(PBYTE page);
    void init(PBYTE start, SIZE size);
private:
    Zone* zone;
    Page* memMap;
};