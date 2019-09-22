#include <Types.h>
#include <global/OS.h>

void _cdecl operator delete(void* addr, SIZE size) {
	if (os == NULL || os->pool == NULL) {
		os->setLastStatus(StatusNullPointer);
		return;
	}
	os->pool->free((PBYTE)addr);
	os->setLastStatus(StatusSuccess);
}

void _cdecl operator delete[](void* addr, SIZE size) {
	if (os == NULL || os->pool == NULL) {
		os->setLastStatus(StatusNullPointer);
		return;
	}
	os->pool->free((PBYTE)addr);
	os->setLastStatus(StatusSuccess);
}