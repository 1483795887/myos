#include <mm/PageMapper.h>

PDE getPDEIndex(ULONG address) {
	return address >> PDE_SHIFT;
}

PTE getPTEIndex(ULONG address) {
	address = address << PDE_BITS;
	address = address >> PDE_BITS;
	return address >> PTE_SHIFT;
}

ULONG getAddressFromEntry(ULONG entry) {
	return ulAlign(entry, PAGE_SIZE, FALSE);
}

PDE makePDE(ULONG address, ULONG flags) {
	return (PDE)ulAlign(address, PAGE_SIZE, FALSE) | (flags & 0xfff);
}

PTE makePTE(ULONG address, ULONG flags) {
	return (PTE)ulAlign(address, PAGE_SIZE, FALSE) | (flags & 0xfff);
}

BOOL isPageExist(ULONG entry)
{
	return (BOOL)(entry & Existence);
}
