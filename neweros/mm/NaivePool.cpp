#include <Status.h>
#include <mm/NaivePool.h>

PBYTE NaivePool::allocate(SIZE size) {
    if (remainedSize < size) {
		status = StatusPoolNotEnough;
        return NULL;
    } else {
        PBYTE result = nextAddress;
        if (((ULONG)result) + size < (ULONG)result) { //超出最大内存
            status = StatusMemoryOverLimit;
            return NULL;
        }

        remainedSize -= size;
        nextAddress += size;
        return result;
    }
}

NaivePool::NaivePool(PBYTE start, SIZE size)
{
	ULONG addr = (ULONG)start;
	this->nextAddress = start;
	this->remainedSize = size;
}
