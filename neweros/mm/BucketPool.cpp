#include <global/OS.h>
#include <klib/Memory.h>
#include <mm/BucketPool.h>

void BucketDirectory::setSize(SIZE size) {
    this->size = size;
}

SIZE BucketDirectory::getSize() {
    return size;
}

BucketEntry* BucketDirectory::getEntry(PBYTE ptr) {
    if (list.getCount() == 0)
        return NULL;
    PBYTE alignedPtr = addressPageAlign(ptr);
    CListEntry* ptrEntry = (CListEntry*)ptr;
    BucketEntry* result = NULL;
    BucketEntry* entry = (BucketEntry*)list.getFirst();
    while (entry != list.getHead()) {
        if (addressPageAlign((PBYTE)entry->nextPtr) == alignedPtr) {
            if (addressPageAlign((PBYTE)ptrEntry->getNext()) == alignedPtr &&   //如果在同一个页但不在已申请的池子中，
                    addressPageAlign((PBYTE)ptrEntry->getPrev()) == alignedPtr) //这里用next和prev进行判断，在申请后
                result = NULL;                                                  //会清空申请的内存，原来的链表指针会被
            else                                                                //清掉，极低概率这块内存刚好写的两个
                result = entry;                                                 //都是池子内存页内，可以认为如果都是
            break;                                                              //就是同页面中尚未申请的内存
        } else
            entry = (BucketEntry*)entry->getNext();
    }
    return result;
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
    if (entry->nextPtr->getNext() != entry->nextPtr) {  //如果不是最后一个
        entry->nextPtr->removeThis();
        entry->nextPtr = entry->nextPtr->getNext();
    }

    memset(ptr, 0, currentSize);

    entry->refCount++;

    os->setLastStatus(Success);
    return ptr;
}

/*
    当池子里一个都没有的时候nextptr就指的是addr
    对addr内作为链表项进行初始化连入自己，
    当只有一个项的时候连入自己就相当于什么事都没有发生
    但为了和有项的时候保持一致，在空的时候也连入
*/
void BucketPool::free(PBYTE addr) {
    BucketDirectory* directory = NULL;
    BucketEntry* entry = getEntry(addr, directory);
    if (entry == NULL)
        return;
    CListEntry* listPtr = (CListEntry*)addr;
    listPtr->setAddress(addr);
    entry->nextPtr->insertNext(listPtr);
    entry->refCount--;
    if (entry->refCount == 0) {
        allocator->putPage((PBYTE)entry->nextPtr);
        entry->removeThis();
    }
}

BOOL BucketPool::isInPool(PBYTE ptr) {
    return (BOOL)(getEntry(ptr, NULL) != NULL);
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
    prev->setAddress((PBYTE)prev);
    CListEntry* next = (CListEntry*)((PBYTE)prev + size);
    for (int i = 1; i < countOfPools; i++) {
        next->setAddress((PBYTE)next);
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

/*
    directory输出entry所在在的目录
    以返回值是否为空来判断是否找到，
    返回值为空时directory无意义
*/
BucketEntry* BucketPool::getEntry(PBYTE ptr, BucketDirectory* directory) {
    BucketEntry* entry = NULL;
    directory = direcories;
    for (int i = 0; i < MAX_POOL_ORDER; i++, directory++) {
        entry = directory->getEntry(ptr);
        if (entry)
            break;
    }
    return entry;
}

void BucketPool::setAllocator(PhysicalPageAllocator* allocator) {
    this->allocator = allocator;
}

BucketEntry::BucketEntry() {
    refCount = 0;
    nextPtr = NULL;
}
