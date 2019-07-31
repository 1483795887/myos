#pragma once
#include "../Types.h"
#include "../Status.h"
#include <lib/CList.h>

#define MAX_ORDER 10

class FreeArea {
public:
    Status LinkFreePages(PBYTE start, ULONG size);
    CList* getList() {
        return &freeList;
    }
private:
    CList freeList;
    PBYTE map;
};

class Zone {
public:
private:
    ULONG start;
    ULONG size;
};