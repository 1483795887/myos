#pragma once

#include "../types.h"
#include "mm.h"

#define PDE_SHIFT 22
#define PTE_SHIFT 12

#define PDE_BITS 10
#define PTE_BITS 10

typedef ULONG   PDE;
typedef PDE*    PD;
typedef ULONG   PTE;
typedef PTE*    PT;

class PhysicalPageManager {
public:
    PhysicalPageManager(PD pd);
    PhysicalPageManager();
    void setPD(PD pd);
    PDE getPDE(ULONG address);
    PTE getPTE(ULONG address);
private:
    PD pd;
};