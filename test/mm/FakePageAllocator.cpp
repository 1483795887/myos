#include "pch.h"
#include "FakePageAllocator.h"
#include <mm/mm.h>

PBYTE FakePageAllocator::allocPages(ULONG order) {
    ULONG number = 1 << order;
    ULONG size = number * PAGE_SIZE;
    if (currentBlock >= MAX_BLOCKS || (remainPages < number))
        return (PBYTE)NULL;
    PBYTE buffer = (PBYTE)_aligned_malloc(size, size);
    memset((void*)buffer, 0, size);
    blocks[currentBlock] = buffer;
    currentBlock++;
    remainPages -= number;
    return buffer;
}

void FakePageAllocator::putPage(PBYTE page) {
    remainPages ++;
}

PBYTE FakePageAllocator::getLastPage() {
    if (currentBlock > 0)
        return blocks[currentBlock - 1];
    return (PBYTE)NULL;
}

void FakePageAllocator::setRemainPages(ULONG remainPages) {
    this->remainPages = remainPages;
}

FakePageAllocator::FakePageAllocator() {
    currentBlock = 0;
    remainPages = -1;
    for (int i = 0; i < MAX_BLOCKS; i++)
        blocks[i] = NULL;
}

FakePageAllocator::~FakePageAllocator() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i] != NULL) {
            _aligned_free((void*)blocks[i]);
            blocks[i] = NULL;
        }

    }
}