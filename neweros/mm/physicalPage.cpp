#include <fs/pe.h>
#include "..\include\mm\physicalPage.h"

inline PDE getPDEIndex(ULONG address) {
    return address >> PDE_SHIFT;
}

inline PTE getPTEIndex(ULONG address) {
    address = address << PDE_BITS;
    address = address >> PDE_BITS;
    return address >> PTE_SHIFT;
}

inline ULONG getAddressFromEntry(ULONG entry) {
    ULONG mask = ~0xfff;
    entry = entry & mask;
    return entry;
}

inline BOOL checkPageAddressValid(ULONG address) {
    if (((ULONG)address) > 4 * G - K)
        return FALSE;
    return TRUE;
}

PhysicalPageManager::PhysicalPageManager(PD pd) {
    this->pd = pd;
}

PhysicalPageManager::PhysicalPageManager() {
    this->pd = (PD)FAILED;
    this->allocator = NULL;
}

void PhysicalPageManager::setPD(PD pd) {
    this->pd = pd;
}

ULONG PhysicalPageManager::va2pa(ULONG vAddr) {
	PDE pde = pd[getPDEIndex(vAddr)];
	if (!(pde & Existence))
		return PDNotExist;
	PT pt = (PT)getAddressFromEntry(pde);
	PTE pte = pt[getPTEIndex(vAddr)];
	if (!(pte & Existence))
		return PTNotExist;
    return getAddressFromEntry(pte);
}

MapPagesStatus PhysicalPageManager::mapPages(ULONG pAddr, ULONG vAddr, ULONG size, ULONG property) {
    pAddr = addressAlign(pAddr, PAGE_SIZE, FALSE);
    vAddr = addressAlign(vAddr, PAGE_SIZE, FALSE);
    size = addressAlign(size, PAGE_SIZE, TRUE);
    property = property & 0xfff;
    if (vAddr > vAddr + size)
        return SizeTooBig;
    ULONG currentSize = 0;
    while (currentSize < size) {
        PDE pde = pd[getPDEIndex(vAddr)];
        if (pde & Existence) {
            PT pt = (PT)getAddressFromEntry(pde);
            PTE pte = pt[getPTEIndex(vAddr)];
            if (pte & Existence)
                return PageAlreadyExist;
            else {
				pte = pAddr | property;
				pt[getPTEIndex(vAddr)] = pte;
            }

        } else { //TODO:申请页面
        }
        currentSize += PAGE_SIZE;
		vAddr += PAGE_SIZE;
		pAddr += PAGE_SIZE;
    }
    return Succeed;
}

void PhysicalPageManager::setZone(Zone zone) {
    this->zone = zone;
}

void PhysicalPageManager::setAllocator(PhysicalPageAllocator* allocator) {
    this->allocator = allocator;
}
