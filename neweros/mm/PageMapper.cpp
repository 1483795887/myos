#include <global/OS.h>
#include <mm/PhysicalPageAllocatorImpl.h>
#include <mm/PageMapper.h>
#include <arch/CPU.h>

/*inline PDE getPDEIndex(ULONG address) {
    return address >> PDE_SHIFT;
}

inline PTE getPTEIndex(ULONG address) {
    address = address << PDE_BITS;
    address = address >> PDE_BITS;
    return address >> PTE_SHIFT;
}

inline ULONG getAddressFromEntry(ULONG entry) {
    return entry & ~0xfff;
}

inline BOOL checkPageAddressValid(ULONG address) {
    if (((ULONG)address) > 4 * G - K)
        return FALSE;
    return TRUE;
}
*/
PageMapper::PageMapper() {
    this->pd = (PD)NULL;
    this->allocator = NULL;
}

void PageMapper::init() {
    this->pd = (PD)allocator->allocPages(0, NOT_ASSIGNED);
}

void PageMapper::setPD(PD pd) {
    this->pd = pd;
}

ULONG PageMapper::va2pa(ULONG vAddr) {
    PDE pde = pd[getPDEIndex(vAddr)];
    if (!(pde & Existence))
        return NULL;
    PT pt = (PT)getAddressFromEntry(pde);
    PTE pte = pt[getPTEIndex(vAddr)];
    if (!(pte & Existence))
        return NULL;
    return getAddressFromEntry(pte);
}

Status PageMapper::mapPages(ULONG pAddr, ULONG vAddr, ULONG size, ULONG property) {
    pAddr = ulAlign(pAddr, PAGE_SIZE, FALSE);
    vAddr = ulAlign(vAddr, PAGE_SIZE, FALSE);
    size = ulAlign(size, PAGE_SIZE, TRUE);
    Status status = StatusSuccess;
    if (vAddr > vAddr + size) {
        status = StatusMemoryOverLimit;
        os->setLastStatus(status);
        return status;
    }
    ULONG currentSize = 0;
    while (currentSize < size) {
        PDE pde = pd[getPDEIndex(vAddr)];
        if (isPageExist(pde)) {
            PT pt = (PT)getAddressFromEntry(pde);
            PTE pte = pt[getPTEIndex(vAddr)];
            if (isPageExist(pte)) {
                status = StatusPageAlreadyExist;
                os->setLastStatus(status);
                return status;
            } else {
				pte = makePTE(pAddr, property);
                pt[getPTEIndex(vAddr)] = pte;
            }
        } else {
            PT pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
			pd[getPDEIndex(vAddr)] = makePDE((ULONG)pt, property);
            continue;
        }
        currentSize += PAGE_SIZE;
        vAddr += PAGE_SIZE;
        pAddr += PAGE_SIZE;
    }
    os->setLastStatus(status);
    return status;
}

void PageMapper::setAllocator(PhysicalPageAllocator* allocator) {
    this->allocator = allocator;
}

void PageMapper::changePD() {
	setPageDirectory(pd);
}
