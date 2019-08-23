#include <global/OS.h>
#include <mm/PhysicalPageAllocatorImpl.h>

PBYTE PhysicalPageAllocatorImpl::allocPages(ULONG order) {
    return zone->getPages(order);
}

void PhysicalPageAllocatorImpl::putPage(PBYTE page) {
    zone->putPage(page);
}

void PhysicalPageAllocatorImpl::init(PBYTE start, SIZE memorySize) {
	ULONG pageCount = memorySize >> LOG2_PAGE_SIZE;
	memMap = New Page[pageCount];
	for (int i = 0; i < pageCount; i++)
		memMap[i].setAddress(start + i * PAGE_SIZE);

	zone = New Zone();
	zone->init(start, memorySize, memMap);
	zone->putAllPages();
}
