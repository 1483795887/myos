#include <global/OS.h>
#include <mm/PhysicalPageAllocator.h>
#include <mm/PageMapper.h>
#include <arch/CPU.h>

ULONG PageMapper::va2pa(PD pd, ULONG vAddr) {
    PDE pde = pd[getPDEIndex(vAddr)];
    if (!(pde & PMExistent))
        return NULL;
    PT pt = (PT)getAddressFromEntry(pde);
    PTE pte = pt[getPTEIndex(vAddr)];
    if (!(pte & PMExistent))
        return NULL;
    return getAddressFromEntry(pte);
}

void _stdcall PageMapper::mapPT(PD pd, ULONG vAddr, ULONG property, PageAllocator * allocator) {
	PDE pde = pd[getPDEIndex(vAddr)];
    if (!isPageExist(pde)) {
        PT pt = (PT)allocator->allocPages(0);
        pd[getPDEIndex(vAddr)] = makePDE((ULONG)pt, property);
    }
}

Status PageMapper::mapPages(PD pd, ULONG pAddr, ULONG vAddr,
                            ULONG size, ULONG property, PageAllocator* allocator) {
    pAddr = ulAlign(pAddr, PAGE_SIZE, FALSE);
    vAddr = ulAlign(vAddr, PAGE_SIZE, FALSE);
    size = ulAlign(size, PAGE_SIZE, TRUE);
    Status status = StatusSuccess;
    if (vAddr > vAddr + size) {
        status = StatusMemoryOverLimit;
        return status;
    }
    ULONG currentSize = 0;
    while (currentSize < size) {
		PageMapper::mapPT(pd, vAddr, property, allocator);
        PDE pde = pd[getPDEIndex(vAddr)];
		PT pt = (PT)getAddressFromEntry(pde);
		PTE pte = pt[getPTEIndex(vAddr)];
		if (isPageExist(pte)) {
			status = StatusPageAlreadyExist;
			return status;
		}
		else {
			pte = makePTE(pAddr, property);
			pt[getPTEIndex(vAddr)] = pte;
		}
        currentSize += PAGE_SIZE;
        vAddr += PAGE_SIZE;
        pAddr += PAGE_SIZE;
    }
    return status;
}

void PageMapper::changePD(PD pd) {
    setPageDirectory(pd);
}
