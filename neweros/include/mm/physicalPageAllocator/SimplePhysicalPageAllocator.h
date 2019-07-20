#pragma once
#include "physicalPageAllocator.h"

class SimplePhysicalPageAllocator : public PhysicalPageAllocator {
public:
    virtual ULONG allocPage(Zone* zone, ULONG order);
};
