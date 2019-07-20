#pragma once
#include "../../types.h"
#include "../zone.h"

class PhysicalPageAllocator {
public:
    virtual ULONG allocPage(Zone* zone, ULONG order) = 0;
};