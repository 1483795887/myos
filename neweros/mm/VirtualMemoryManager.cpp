#include <global/OS.h>
#include <mm/VirtualMemoryManager.h>

VirtualMemoryManager::VirtualMemoryManager() {
    VirtualMemoryArea* unavaliableArea = New VirtualMemoryArea(0, MIN_MEMORY, VMNotAvaliable);
    areaList.insertHead(unavaliableArea);

    VirtualMemoryArea* wholeInfo = New VirtualMemoryArea(MIN_MEMORY, MAX_MEMORY - MIN_MEMORY, VMFree);
    areaList.insertAfter(unavaliableArea, wholeInfo);
}

ULONG VirtualMemoryManager::allocate(SIZE size, ULONG property) {
    if (!checkAligned(size, PAGE_SIZE))
        return 0;
    VirtualMemoryArea* area = getFreeArea(size);
    if (area == NULL)
        return 0;
    divide(area, area->startAddr, size, property);
    return area->startAddr;
}

void VirtualMemoryManager::release(ULONG addr, SIZE size) {
    if (!checkAligned(addr, PAGE_SIZE))
        return;
    if (!checkAligned(size, PAGE_SIZE))
        return;

    VirtualMemoryArea* area = getAreaByAddress(addr);
    if (area->property != VMNotAvaliable)
        divide(area, addr, size, VMFree);

}

VirtualMemoryArea * VirtualMemoryManager::getFreeArea(SIZE size) {
    VirtualMemoryArea* area = (VirtualMemoryArea*)areaList.getFirst();
    BOOL find = FALSE;
    while (area != areaList.getHead()) {
        if (area->size >= size && area->property == VMFree) {
            find = TRUE;
            break;
        }
        area = (VirtualMemoryArea*)area->getNext();
    }
    if (!find)
        area = NULL;
    return area;
}

//这个函数不会出现找不到的情况
VirtualMemoryArea * VirtualMemoryManager::getAreaByAddress(ULONG addr) {
    VirtualMemoryArea* area = (VirtualMemoryArea*)areaList.getFirst();
    while (area != areaList.getHead()) {
        if (area->startAddr <= addr && area->startAddr + area->size > addr)
            break;
        area = (VirtualMemoryArea*)area->getNext();
    }
    return area;
}

void VirtualMemoryManager::divide(VirtualMemoryArea * area,
                                  ULONG startAddr, SIZE size, ULONG newProperty) {

    if (startAddr < area->startAddr ||
            startAddr >= area->size + area->startAddr ||
            size > area->size ||
            area->property == VMNotAvaliable)
        return;

    VirtualMemoryArea* before = NULL, *after = NULL;
    VirtualMemoryArea* cur = (VirtualMemoryArea*)area->getPrev();
    VirtualMemoryArea* last = (VirtualMemoryArea*)area->getNext();

    before = New VirtualMemoryArea(area->startAddr, startAddr - area->startAddr, area->property);
    after = New VirtualMemoryArea(startAddr + size,
                                  area->startAddr + area->size - startAddr - size, area->property);

    area->property = newProperty;
    area->size = size;
    area->startAddr = startAddr;

    areaList.insertBefore(area, before);
    areaList.insertAfter(area, after);

    if (last == areaList.getHead())
        last = after; //last的下一个肯定不会是head

    while (cur->startAddr + cur->size != last->startAddr + last->size) {
        VirtualMemoryArea* next = (VirtualMemoryArea*)cur->getNext();
        if (merge(cur, next)) {
            areaList.remove(next);
            delete next;
        } else
            cur = (VirtualMemoryArea*)cur->getNext();
    }
}

BOOL VirtualMemoryManager::merge(VirtualMemoryArea * before, VirtualMemoryArea * next) {
    if (next->size == 0)
        return TRUE;
    else if (before->property == next->property) {
        before->size += next->size;
        return TRUE;
    } else
        return FALSE;
}
