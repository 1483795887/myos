#pragma once
#include <Status.h>
#include <mm/Mm.h>
#include <lib/CList.h>
#include <lib/CListEntry.h>

#define MIN_MEMORY 1 * M

typedef enum {
    VMFree = 0,
    VMWritable = 1,
    VMExecutable = 2,
    VMNotAvaliable = 0xff
} VirtualMemoryAreaProperty;

class VirtualMemoryArea : public CListEntry {
public:
    VirtualMemoryArea(ULONG addr, SIZE size, ULONG property) : startAddr(addr),
        size(size), property(property) {

    }
    VirtualMemoryArea(): property(VMFree) {

    }

    ULONG startAddr;
    SIZE size;
    ULONG property;
};

class VirtualMemoryManager {
public:
    ULONG allocate(SIZE size, ULONG property);
    void release(ULONG addr, SIZE size);
    VirtualMemoryManager();
private:
    VirtualMemoryArea* getFreeArea(SIZE size);
    VirtualMemoryArea* getAreaByAddress(ULONG addr);

    void divide(VirtualMemoryArea* area, ULONG startAddr, SIZE size, ULONG newProperty);
	BOOL merge(VirtualMemoryArea* before, VirtualMemoryArea* next);


    CList areaList;
};
