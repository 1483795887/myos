#pragma once

#include "PhysicalPageAllocator.h"

class PhysicalPageAllocatorImpl : public PhysicalPageAllocator {
public:
    virtual PBYTE allocPages(Zone* zone, ULONG order);
    virtual~PhysicalPageAllocatorImpl() {

    }
};