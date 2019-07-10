#include <types.h>
#include <fs/pe.h>
#include <klib/kememory.h>
#include "setup.h"

void copyKernel() {
	PBYTE address = (PBYTE)KernelTempBase;
	DosHeader* kernelHeader = (DosHeader*)address;

	address += kernelHeader->lfanew;
	NtHeader* ntHeader = (NtHeader*)address;

	address += sizeof(NtHeader);
	OptionalHeader* optionalHeader = (OptionalHeader*)address;

	DWORD imageBase = optionalHeader->imageBase;
	DWORD entryAddress = imageBase + optionalHeader->addressOfEntryPoint;
	DWORD numberOfSections = ntHeader->numberOfSections;

	address += ntHeader->sizeOfOptionalHeader;
	Section * sections = (Section*)address;

	KeMemCopy((PBYTE)KernelTempBase, (PBYTE)KernelImageBase, sections[0].rawAddress);

	for (int i = 0; i < numberOfSections; i++) {
		KeMemCopy(
			(PBYTE)(KernelTempBase + sections[i].rawAddress),
			(PBYTE)(KernelImageBase + sections[i].virtualAddress),
			sections[i].rawSize);
	}
}

void main() {
	
	copyKernel();


	for (;;);
}