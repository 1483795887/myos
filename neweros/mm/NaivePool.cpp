#include <mm/NaivePool.h>
#include <global/OS.h>


PBYTE NaivePool::allocate(SIZE size) {
    if (remainedSize < size) {
        os->setLastStatus(StatusPoolNotEnough);
        return NULL;
    } else {
        PBYTE result = nextAddress;
        if (((ULONG)result) + size < (ULONG)result) { //超出最大内存
            os->setLastStatus(StatusMemoryOverLimit);
            return NULL;
        }

        remainedSize -= size;
        nextAddress += size;
		os->setLastStatus(StatusSuccess);
        return result;
    }
}

NaivePool::NaivePool(PBYTE start, SIZE size)
{
	ULONG addr = (ULONG)start;
	this->nextAddress = start;
	this->remainedSize = size;
}
