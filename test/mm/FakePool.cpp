#include "pch.h"
#include "FakePool.h"

#define MAX_POOL_SIZE 1024

PBYTE FakePool::allocate(SIZE size) {
    if (remainSize < size)
        return NULL;
    else {
        PBYTE result = current;
        current += size;
        remainSize -= size;
        return result;
    }
}

FakePool::FakePool() {
    pool = (PBYTE)malloc(MAX_POOL_SIZE);
    memset(pool, 0, MAX_POOL_SIZE);
    current = pool;
    remainSize = MAX_POOL_SIZE;
}

FakePool::~FakePool() {
    free(pool);
    remainSize = 0;
    current = NULL;
}
