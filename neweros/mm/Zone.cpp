#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/Zone.h>
#include <mm/Page.h>

Status FreeArea::init(PBYTE address, ULONG order, ULONG memorySize) {
    Status status;
    if (order > MAX_ORDER) {
        status = ValueNotInRange;
        os->setLastStatus(status);
        return status;
    }
    this->order = order;
    this->start = address;

    map.init(memorySize >> (order + LOG2_PAGE_SIZE + 1));
    return os->getLastStatus();
}

ULONG FreeArea::getCount() {
    return freeList.getCount();
}

void FreeArea::insert(Page* page) {
    freeList.insertHead(page);
    if (order != MAX_ORDER)
        map.revBit(getPageNoByAddress(page->getAddress()) >> 1);
}

void FreeArea::remove(Page* page) {
    freeList.remove(page);
    if (order != MAX_ORDER)
        map.revBit(getPageNoByAddress(page->getAddress()) >> 1);
}

BOOL FreeArea::canMerge(PBYTE address) {
    return NOT(map.checkBit(getPageNoByAddress(address) >> 1));
}

//获得第一个页并且从链表中移除
Page* FreeArea::getFirst() {
    Page* first = (Page*)freeList.getFirst();
    if (first != (Page*)freeList.getHead()) {
        freeList.remove(first);
        return first;
    }
    return NULL;
}

ULONG FreeArea::getPageNoByAddress(PBYTE address) {
    return (ULONG)(address - start) / getPageSizeByOrder(order);
}

Status Zone::init(PBYTE start, ULONG memorySize, Page* pages) {
    this->start = (PBYTE)ulAlign((ULONG)start, getPageSizeByOrder(MAX_ORDER), TRUE);
    this->size = ulAlign(memorySize, PAGE_SIZE, FALSE);

    ULONG pageSize = PAGE_SIZE;
    for (int i = 0; i < MAX_ORDER + 1; i++) {
        ULONG alignedSize = ulAlign(memorySize, pageSize, FALSE);
        if (alignedSize == 0 || freeAreas[i].init((PBYTE)ulAlign((ULONG)start, pageSize, TRUE), i, alignedSize) != Success)
            break;
        pageSize <<= 1;
    }

    this->memMap = pages;
	for (int i = 0; i < size >> LOG2_PAGE_SIZE; i++)
		memMap[i].setCount(1);

    return os->getLastStatus();
}

ULONG Zone::getFreePages() {
    return 0;
}

Status Zone::putPage(PBYTE address) {
    Status status = Success;
    if (!checkAligned((ULONG)address, PAGE_SIZE)) {
        status = InvalidPage;
        os->setLastStatus(status);
        return status;
    }
    if (address < start || address >= start + size) {
        status = InvalidPage;
        os->setLastStatus(status);
        return status;
    }
    Page* page = getPageByAddress(address);
    page->setAddress(address);//防止地址没有初始化
	if (page->getCount() <= 0) 
		os->setLastStatus(PageAlreadyFreeed);
	else {
		page->decCount();
		if (page->getCount() == 0)
			mergePage(page);
	}
    return os->getLastStatus();
}

Status Zone::putAllPages() {
    ULONG currentSize;
    for (currentSize = 0; currentSize < size; currentSize += PAGE_SIZE) {
        if (putPage(start + currentSize) != Success)
            break;
    }
    return os->getLastStatus();
}

ULONG Zone::getFreePagesForOrder(int order) {
    if (order < 0 || order > MAX_ORDER)
        return 0;
    return freeAreas[order].getCount();
}

void Zone::addCount(PBYTE addr, ULONG order) {
    PBYTE lastAddr = addr + getPageSizeByOrder(order) ;
    for (PBYTE curr = addr; curr < lastAddr; curr += PAGE_SIZE) {
        Page* page = getPageByAddress(curr);
        page->setCount(1);
    }
}

PBYTE Zone::getPages(ULONG order) {
    if (order > MAX_ORDER) {
        os->setLastStatus(ValueNotInRange);
        return NULL;
    }
    if (freeAreas[order].getCount() != 0) {
        Page* page = freeAreas[order].getFirst();
        addCount(page->getAddress(), order);
        return page->getAddress();
    }
    return dividePage(order);
}

void Zone::mergePage(Page* page) {
    Page* currentPage = page;
    int currentOrder = 0;
    ULONG pageSize = PAGE_SIZE;

    freeAreas[currentOrder].insert(currentPage);

    while (currentOrder < MAX_ORDER &&
		getPageSizeByOrder(currentOrder) <= size / 2 &&
		freeAreas[currentOrder].canMerge(currentPage->getAddress())
		) {

        currentPage = getPageByAddress((PBYTE)ulAlign((ULONG)currentPage->getAddress(), pageSize << 1, FALSE));
        freeAreas[currentOrder].remove(currentPage);
        freeAreas[currentOrder].remove(getPageByAddress(currentPage->getAddress() + pageSize));
        currentOrder++;

        pageSize <<= 1;

        freeAreas[currentOrder].insert(currentPage);
    }
}

PBYTE Zone::dividePage(ULONG order) {
    Page* page = NULL;
    ULONG i;
    for (i = order + 1; i <= MAX_ORDER; i++) {
        if (freeAreas[i].getCount() != 0) {
            page = freeAreas[i].getFirst();
            break;
        }
    }
    for (; i > order && page != NULL; i--) {
        PBYTE higherAddress = page->getAddress() + getPageSizeByOrder(i - 1);
        Page* higherPage = getPageByAddress(higherAddress);
        freeAreas[i - 1].insert(higherPage);
    }
    if (page != NULL) {
        addCount(page->getAddress(), order);
        return page->getAddress();
    }
    return NULL;
}

Page* Zone::getPageByAddress(PBYTE address) {
    return &memMap[(ULONG)(address - start) >> LOG2_PAGE_SIZE];
}

