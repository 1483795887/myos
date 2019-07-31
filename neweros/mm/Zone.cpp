#include <mm/Mm.h>
#include <mm/Zone.h>
#include <mm/Page.h>

Status FreeArea::LinkFreePages(PBYTE start, ULONG size) {
	ULONG address = (ULONG)start;
	if (address & 0x3ff != 0)
		return AddressNotAligned;
	if ((size & 0x3ff) != 0)
		return AddressNotAligned;
	if (address > address + size)
		return MemoryOverLimit;
	ULONG offset = 0;
	for (offset = 0; offset < size; offset++) {
		Page* page = (Page*)(start + offset);
		page->setData(start + offset);
		freeList.insertHead(page);
		offset += PAGE_SIZE;
	}
    return Success;
}