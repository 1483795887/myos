#include <mm/NaivePool.h>

PBYTE NaivePool::allocate(SIZE size) {
	if (remainedSize < size)
		return NULL;
	else {
		PBYTE result = nextAddress;
		if (((ULONG)result) + size < (ULONG)result) //超出最大内存
			return NULL;
		remainedSize -= size;
		nextAddress += size;
		return result;
	}
}

NaivePool::NaivePool(PBYTE start, ULONG size) {
    ULONG addr = (ULONG)start;
    this->nextAddress = start;
    this->remainedSize = size;
}
