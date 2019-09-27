#pragma once

#include <mm/PageAllocator.h>
#include <types.h>

#define MAX_BLOCKS 10
class FakePageAllocator : public PageAllocator {
public:
    virtual PBYTE allocPages(ULONG order);
	virtual void putPage(PBYTE page);

    PBYTE getLastPage();

	void setRemainPages(ULONG remainPages);

    FakePageAllocator();

    virtual ~FakePageAllocator();
private:
    PBYTE blocks[MAX_BLOCKS];
    ULONG currentBlock;
	ULONG remainPages;
};