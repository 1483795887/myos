#pragma once

#include "../Types.h"
#include "../Status.h"
#include "Mm.h"
#include "Zone.h"
#include "PhysicalPageAllocator.h"

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
	Supervisor= 4
};

class PhysicalPageManager {
public:
    PhysicalPageManager(PD pd);
    PhysicalPageManager();
    void setPD(PD pd);
	ULONG va2pa(ULONG vAddr);
	Status mapPages(ULONG pAddr, ULONG vAddr, ULONG size, ULONG property);
    void setZone(Zone zone);
    void setAllocator(PhysicalPageAllocator* allocator);
private:
    PD pd;
    Zone zone;
    PhysicalPageAllocator* allocator;
	
};