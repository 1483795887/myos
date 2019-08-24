#pragma once

#include <mm/physicalPage.h>
#include <mm/physicalPageAllocator.h>
#include <types.h>

#define MAX_BLOCKS 10
class FakePhysicalPageAllocator : public PhysicalPageAllocator {
public:
    virtual PBYTE allocPages(ULONG order);
	virtual void putPage(PBYTE page);

    PBYTE getLastPage();

	void setRemainPages(ULONG remainPages);

    FakePhysicalPageAllocator();

    virtual ~FakePhysicalPageAllocator();
private:
    PBYTE blocks[MAX_BLOCKS];
    ULONG currentBlock;
	ULONG remainPages;
};