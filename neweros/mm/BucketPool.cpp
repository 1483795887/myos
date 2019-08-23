#include <global/OS.h>
#include <mm/BucketPool.h>

void BucketDirectory::setSize(SIZE size) {
    this->size = size;
}

SIZE BucketDirectory::getSize() {
    return size;
}

BucketEntry* BucketDirectory::getFreeBucketEntry() {
    if (list.isEmpty())
        return NULL;
    BucketEntry* first = (BucketEntry*)list.getFirst();
    while (first != (BucketEntry*)list.getHead()) {
        if (first->refCount < getMaxRefCount())
            break;
        first = (BucketEntry*)first->getNext();
    }
    if (first == list.getHead())    //所有的entry都满了
        return NULL;
    return first;
}

ULONG BucketDirectory::getMaxRefCount() {
    return PAGE_SIZE / size;
}


BucketPool::BucketPool() {
    initialized = FALSE;
    for (int i = 0; i < MAX_POOL_ORDER; i++)
        direcories[i].setSize(MIN_POOL_SIZE << i);
}

PBYTE BucketPool::allocate(SIZE size) {
    if (!initialized) {  //第一次申请的时候要进行初始化
        BOOL result = init();
        if (!result)
            return NULL;
    }
    if (size > (MIN_POOL_SIZE << MAX_POOL_ORDER)) {
        os->setLastStatus(SizeTooBig);
        return NULL;
    }

    ULONG currentSize = MIN_POOL_SIZE;
    int order;
    for (order = 0; order < MAX_POOL_ORDER; order++) {
        if (currentSize >= size)
            break;
        currentSize <<= 1;
    }

    BucketEntry* entry = direcories[order].getFreeBucketEntry();
    if (entry == NULL) {//没有空的entry，尝试申请一个
		entry = allocateBucketEntry(currentSize);
		if (entry == NULL)
			return NULL;
		direcories[order].list.insertHead(entry);
    }

    PBYTE ptr = (PBYTE)entry->nextPtr;
    entry->nextPtr = entry->nextPtr->getNext();
    entry->refCount++;

    os->setLastStatus(Success);
    return ptr;
}

void BucketPool::free(PBYTE addr) {
}

BOOL BucketPool::isInPool(PBYTE ptr) {
    return FALSE;
}

BOOL BucketPool::init() {
    BucketEntry* entries = (BucketEntry*)allocator->allocPages(getOrderByPageSize(MAX_POOL_PAGES * PAGE_SIZE));
    if (entries == NULL) {
        os->setLastStatus(PoolNotEnough);
        return FALSE;
    }
    for (int i = 0; i < PAGE_SIZE * MAX_POOL_PAGES / sizeof(BucketEntry); i++) {
        entries[i] = BucketEntry();
        entries[i].nextPtr = NULL;
        entryList.insertHead(&entries[i]);
    }
    initialized = TRUE;
    return TRUE;
}

PBYTE BucketPool::getNewPoolPage(SIZE size) {
    if (allocator == NULL) {
        os->setLastStatus(NullPointer);
        return NULL;
    }
    if (size == 0 || size > (MIN_POOL_SIZE << MAX_POOL_ORDER)) {
        os->setLastStatus(ParameterError);
        return NULL;
    }
    PBYTE ptr = allocator->allocPages(0);
    if (ptr == NULL) {
        os->setLastStatus(PoolNotEnough);
        return NULL;
    }
    ULONG countOfPools = PAGE_SIZE / size;
    CListEntry* prev = (CListEntry*)ptr;
    *prev = CListEntry();
    CListEntry* next = (CListEntry*)((PBYTE)prev + size);
    for (int i = 1; i < countOfPools; i++) {
        *next = CListEntry();
        prev->insertNext(next);
        prev = (CListEntry*)((PBYTE)prev + size);
        next = (CListEntry*)((PBYTE)next + size);
    }
    os->setLastStatus(Success);
    return ptr;
}

BucketEntry* BucketPool::allocateBucketEntry(SIZE size) {
    if (entryList.getCount() == 0) {//所有的entry都用完了
        os->setLastStatus(PoolNotEnough);
        return NULL;
    }
    BucketEntry* entry = (BucketEntry*)entryList.getFirst();
    entryList.remove(entry);
    PBYTE poolPage = getNewPoolPage(size);
    if (poolPage == NULL) {
        os->setLastStatus(PoolNotEnough);
        return NULL;
    }
    entry->nextPtr = (CListEntry*)poolPage;
    entry->refCount = 0;
	return entry;
}

void BucketPool::setAllocator(PhysicalPageAllocator* allocator) {
    this->allocator = allocator;
}

BucketEntry::BucketEntry() {
    refCount = 0;
    nextPtr = NULL;
}
