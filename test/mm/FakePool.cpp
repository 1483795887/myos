#include "pch.h"
#include "FakePool.h"
#include <global/OS.h>

#define MAX_POOL_SIZE 1024

PBYTE FakePool::allocate(SIZE size) {
	if (remainSize < size) {
		os->setLastStatus(PoolNotEnough);
		return NULL;
	}
    else {
        PBYTE result = current;
        current += size;
        remainSize -= size;
        return result;
    }
}

FakePool::FakePool(SIZE size) {
    pool = (PBYTE)malloc(size);
    memset(pool, 0, size);
    current = pool;
    remainSize = size;
}

FakePool::~FakePool() {
    free(pool);
    remainSize = 0;
    current = NULL;
}
