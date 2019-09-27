#pragma once

#include "Pool.h"
#include "PageAllocator.h"

#define MAX_POOL_PAGES 4


class BucketEntry : public CListEntry {
public:
	BucketEntry();

	ULONG refCount;
	CListEntry* nextPtr;
};

class BucketDirectory {
public:
	void setSize(SIZE size);
	SIZE getSize();
	BucketEntry* getEntry(PBYTE ptr);
	CList list;
	BucketEntry* getFreeBucketEntry();
private:
	ULONG getMaxRefCount();
    SIZE size;
};



#define MAX_POOL_ORDER 8
#define MIN_POOL_SIZE 16

class BucketPool : public Pool {
public:
    virtual PBYTE allocate(SIZE size);
    virtual void free(PBYTE addr);
    virtual BOOL isInPool(PBYTE ptr);
	
    void setAllocator(PageAllocator* allocator);
    BucketPool();
private:
	BOOL init();
	PBYTE getNewPoolPage(SIZE size);
	BucketEntry* allocateBucketEntry(SIZE size);

	BucketEntry* getEntry(PBYTE ptr, BucketDirectory* directory);

    PageAllocator* allocator;
    BucketDirectory direcories[MAX_POOL_ORDER + 1];
	CList entryList;
	BOOL initialized;
};