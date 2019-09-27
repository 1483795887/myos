#pragma once
#include <Status.h>
#include <mm/Mm.h>
#include <lib/CList.h>
#include <lib/CListEntry.h>

typedef enum {
    VMNotInMemory = 0,
    VMWritable = 1,
    VMExecutable = 2,
    VMNotAvaliable = 0xff
} VirtualMemoryInfoInfos;

class VirtualMemoryInfo : public CListEntry {
public:
    VirtualMemoryInfo(ULONG addr, SIZE size, ULONG property) : startAddr(addr),
		size(size), property(property) {

    }
	VirtualMemoryInfo():property(VMNotInMemory) {

	}

    ULONG startAddr;
    SIZE size;
    ULONG property;
};

class VirtualMemoryManager {
public:
    Status addMemoryArea(VirtualMemoryInfo info);
    VirtualMemoryInfo* getVirtualMemoryInfo(ULONG addr);
private:
    CList infoList;
};
