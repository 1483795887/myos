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
	Page* getFirst();
private:
    ULONG order;
    CList freeList;
    Bitmap map;
    PBYTE start;

    ULONG getPageNoByAddress(PBYTE address);
};

class Zone {
public:
    Status init(PBYTE start, ULONG memorySize, Page* pages);
    ULONG getFreePages();
	Status putPage(PBYTE start);
	Status putAllPages();
    ULONG getFreePagesForOrder(int order);

	PBYTE getPages(ULONG order);

	Page* getPageByAddress(PBYTE address);
private:
    PBYTE start;
    ULONG size;
    FreeArea freeAreas[MAX_ORDER + 1];
	Page* memMap;

    void mergePage(Page* page);
	PBYTE dividePage(ULONG order);
	void addCount(PBYTE addr, ULONG order);
	
};