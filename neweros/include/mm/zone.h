#pragma once
#include "../Types.h"
#include "../Status.h"
#include <lib/CList.h>
#include <lib/Bitmap.h>

#define MAX_ORDER 10

class FreeArea {
public:
	Status init(ULONG order, ULONG memorySize);
    //Status LinkFreePages(PBYTE start, ULONG size);
    CList* getList() {
        return &freeList;
    }
private:
	ULONG order;
    CList freeList;
    Bitmap map;
};

class Zone {
public:
	Status init(PBYTE start, ULONG memorySize);
	ULONG getFreePages();
private:
    ULONG start;
    ULONG size;
	FreeArea freeAreas[MAX_ORDER];
};