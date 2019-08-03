#include "..\include\mm\PhysicalPage.h"

inline PDE getPDEIndex(ULONG address) {
    return address >> PDE_SHIFT;
}

inline PTE getPTEIndex(ULONG address) {
    address = address << PDE_BITS;
    address = address >> PDE_BITS;
    return address >> PTE_SHIFT;
}

inline ULONG getAddressFromEntry(ULONG entry) {
    return entry & ~0xfff;;
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
    this->pd = (PD)NULL;
    this->allocator = NULL;
}

void PhysicalPageManager::setPD(PD pd) {
    this->pd = pd;
}

ULONG PhysicalPageManager::va2pa(ULONG vAddr) {
	PDE pde = pd[getPDEIndex(vAddr)];
	if (!(pde & Existence))
		return NULL;
	PT pt = (PT)getAddressFromEntry(pde);
	PTE pte = pt[getPTEIndex(vAddr)];
	if (!(pte & Existence))
		return NULL;
    return getAddressFromEntry(pte);
}

Status PhysicalPageManager::mapPages(ULONG pAddr, ULONG vAddr, ULONG size, ULONG property) {
    pAddr = ulAlign(pAddr, PAGE_SIZE, FALSE);
    vAddr = ulAlign(vAddr, PAGE_SIZE, FALSE);
    size = ulAlign(size, PAGE_SIZE, TRUE);
    property = property & 0xfff;
    if (vAddr > vAddr + size)
        return MemoryOverLimit;
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

        } else { 
			PT pt = (PT)allocator->allocPages(&zone, 0);
			pd[getPDEIndex(vAddr)] = (PDE)((ULONG)pt | property);
			continue;
        }
        currentSize += PAGE_SIZE;
		vAddr += PAGE_SIZE;
		pAddr += PAGE_SIZE;
    }
    return Success;
}

void PhysicalPageManager::setZone(Zone zone) {
    this->zone = zone;
}

void PhysicalPageManager::setAllocator(PhysicalPageAllocator* allocator) {
    this->allocator = allocator;
}
