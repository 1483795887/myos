#include <Types.h>
#include <global/OS.h>

void _cdecl operator delete(void* addr, SIZE size) {
	if (os == NULL || os->pool == NULL) {
		os->setLastStatus(NullPointer);
		return;
	}
	os->pool->free((PBYTE)addr);
	os->setLastStatus(Success);
}

void _cdecl operator delete[](void* addr, SIZE size) {
	if (os == NULL || os->pool == NULL) {
		os->setLastStatus(NullPointer);
		return;
	}
	os->pool->free((PBYTE)addr);
	os->setLastStatus(Success);
}