#include <global/OS.h>
#include <mm/PhysicalPageAllocatorImpl.h>

PBYTE PhysicalPageAllocatorImpl::allocPages(Zone* zone, ULONG order) {
	return zone->getPages(order);
}
