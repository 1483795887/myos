#pragma once
#include "../Types.h"
#include "../Status.h"
#include <lib/CList.h>
#include <lib/Bitmap.h>
#include "Page.h"

#define MAX_ORDER 10

class FreeArea {
public:
    Status init(PBYTE start, ULONG order, ULONG memorySize);
    ULONG getCount();
    void insert(Page* page);
    void remove(Page* page);
    BOOL canMerge(PBYTE offset);
private:
    ULONG order;
    CList freeList;
    Bitmap map;
    PBYTE start;

    ULONG getPageNoByAddress(PBYTE address);
};

class Zone {
public:
    Status init(PBYTE start, ULONG memorySize);
    ULONG getFreePages();
    Status putPages(Page* page);

    ULONG getFreePagesForOrder(int order);
private:
    PBYTE start;
    ULONG size;
    FreeArea freeAreas[MAX_ORDER + 1];

    void mergePage(Page* page);
};