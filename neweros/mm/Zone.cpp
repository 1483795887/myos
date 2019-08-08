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
    if (page->getOrder() != MAX_ORDER)
        map.revBit(getPageNoByAddress(page->getAddress()) >> 1);
}

void FreeArea::remove(Page* page) {
    freeList.remove(page);
    if (page->getOrder() != MAX_ORDER)
        map.revBit(getPageNoByAddress(page->getAddress()) >> 1);
}

BOOL FreeArea::canMerge(PBYTE address) {
    return NOT(map.checkBit(getPageNoByAddress(address) >> 1));
}

ULONG FreeArea::getPageNoByAddress(PBYTE address) {
    return (ULONG)(address - start) / getPageSizeByOrder(order);
}

Status Zone::init(PBYTE start, ULONG memorySize) {
    this->start = start;
    this->size = ulAlign(memorySize, PAGE_SIZE, FALSE);

    ULONG pageSize = PAGE_SIZE;
    for (int i = 0; i < MAX_ORDER + 1; i++) {
        freeAreas[i].init((PBYTE)ulAlign((ULONG)start, pageSize, TRUE), i, ulAlign(memorySize, pageSize, FALSE));
        pageSize <<= 1;
    }

    return os->getLastStatus();
}

ULONG Zone::getFreePages() {
    return 0;
}

Status Zone::putPages(Page* page) {
    Status status = Success;
    if (page->getOrder() > MAX_ORDER) {
        status = ValueNotInRange;
        os->setLastStatus(status);
        return status;
    }
    SIZE pageSize = 1 << (page->getOrder() + LOG2_PAGE_SIZE);
    if (!checkAligned((ULONG)page->getAddress(), pageSize)) {
        status = InvalidPage;
        os->setLastStatus(status);
        return status;
    }
    if ((ULONG)page->getAddress() < (ULONG)start
            || (ULONG)page->getAddress() > (ULONG)start + size) {
        status = InvalidPage;
        os->setLastStatus(status);
        return status;
    }
    mergePage(page);
    return os->getLastStatus();
}

ULONG Zone::getFreePagesForOrder(int order) {
    if (order < 0 || order > MAX_ORDER)
        return 0;
    return freeAreas[order].getCount();
}

void Zone::mergePage(Page* page) {
    Page* currentPage = page;
    int currentOrder = page->getOrder();
    ULONG pageSize = getPageSizeByOrder(currentOrder);

    freeAreas[currentOrder].insert(currentPage);

    while (currentOrder < MAX_ORDER && freeAreas[currentOrder].canMerge(currentPage->getAddress())) {

        currentPage = (Page*)(ulAlign((ULONG)currentPage->getAddress(), pageSize << 1, FALSE));
        freeAreas[currentOrder].remove(currentPage);
        freeAreas[currentOrder].remove((Page*)((ULONG)currentPage + pageSize));
        currentOrder++;

        pageSize <<= 1;
        currentPage->setOrder(currentOrder);

        freeAreas[currentOrder].insert(currentPage);
    }
}

