#include <global/OS.h>
#include <mm/VirtualMemoryManager.h>

Status VirtualMemoryManager::addMemoryArea(VirtualMemoryInfo info) {
	if (!checkAligned(info.startAddr, PAGE_SIZE) || !checkAligned(info.size, PAGE_SIZE))
		return StatusNotAligned;

	VirtualMemoryInfo* pInfo = New VirtualMemoryInfo(info.startAddr, info.size, info.property);
	infoList.insertHead(pInfo);

	return StatusSuccess;
}

VirtualMemoryInfo* VirtualMemoryManager::getVirtualMemoryInfo(ULONG addr) {
	addr = ulAlign(addr, PAGE_SIZE, FALSE);
	CListEntry* entry = infoList.getFirst();
	while (entry != infoList.getHead()) {
		VirtualMemoryInfo* info = (VirtualMemoryInfo*)entry;
		if (info->startAddr == addr)
			return info;
		entry = entry->getNext();
	}
    return NULL;
}
