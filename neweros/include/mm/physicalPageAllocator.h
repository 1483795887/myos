#pragma once

#include "PageAllocator.h"

class PhysicalPageAllocator : public PageAllocator {
public:
    virtual PBYTE allocPages(ULONG order);
    virtual void putPage(PBYTE page);
    void init(PBYTE start, SIZE size);
private:
    Zone* zone;
    Page* memMap;
	ULONG baseAddr;
};