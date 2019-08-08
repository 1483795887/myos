#pragma once

#include <mm/physicalPage.h>
#include <mm/physicalPageAllocator.h>
#include <types.h>

#define MAX_BLOCKS 10
class FakePhysicalPageAllocator : public PhysicalPageAllocator {
public:
    virtual PBYTE allocPages(Zone* zone, ULONG order);

    PBYTE getLastPage();

    FakePhysicalPageAllocator();

    virtual ~FakePhysicalPageAllocator();
private:
    PBYTE blocks[MAX_BLOCKS];
    ULONG currentBlock;

};