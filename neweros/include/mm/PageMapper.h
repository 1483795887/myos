#pragma once

#include "../Types.h"
#include "../Status.h"
#include "Mm.h"
#include "Zone.h"
#include "PageAllocator.h"

#define PDE_SHIFT 22
#define PTE_SHIFT 12

#define PDE_BITS 10
#define PTE_BITS 10

typedef ULONG   PDE;
typedef PDE*    PD;
typedef ULONG   PTE;
typedef PTE*    PT;

enum {
    Existence = 1,
    Writable  = 2,
    Supervisor = 4
};

PDE getPDEIndex(ULONG address);
PTE getPTEIndex(ULONG address);
ULONG getAddressFromEntry(ULONG entry);
PDE makePDE(ULONG address, ULONG flags);
PTE makePTE(ULONG address, ULONG flags);
BOOL isPageExist(ULONG entry);

extern "C" void _cdecl setPageDirectory(PD pd);

class PageMapper {
public:
    static ULONG _stdcall va2pa(PD pd, ULONG vAddr);
    static void _stdcall mapPT(PD pd, ULONG vAddr, ULONG property, PageAllocator* allocator);
    static Status _stdcall mapPages(PD pd, ULONG pAddr, ULONG vAddr, ULONG size, ULONG property, PageAllocator* allocator);
    static void _stdcall changePD(PD pd);
};

