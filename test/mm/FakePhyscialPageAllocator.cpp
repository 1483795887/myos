#include "pch.h"
#include "FakePhysicalPageAllocator.h"

PBYTE FakePhysicalPageAllocator::allocPages(Zone* zone, ULONG order) {
    ULONG number = 1 << order;
    ULONG size = number * PAGE_SIZE;
    if (currentBlock >= MAX_BLOCKS)
        return (PBYTE)NULL;
	PBYTE buffer = (PBYTE)_aligned_malloc(size, PAGE_SIZE);
    memset((void*)buffer, 0, size);
    blocks[currentBlock] = buffer;
    currentBlock++;
    return buffer;
}

PBYTE FakePhysicalPageAllocator::getLastPage() {
    if (currentBlock > 0)
        return blocks[currentBlock - 1];
    return (PBYTE)NULL;
}

FakePhysicalPageAllocator::FakePhysicalPageAllocator() {
    currentBlock = 0;
    for (int i = 0; i < MAX_BLOCKS; i++)
        blocks[i] = NULL;
}

FakePhysicalPageAllocator::~FakePhysicalPageAllocator() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i] != NULL) {
            _aligned_free((void*)blocks[i]);
            blocks[i] = NULL;
        }

    }
}