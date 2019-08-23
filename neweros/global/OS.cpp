#include <global/OS.h>

OS* os = NULL;

Status OS::getLastStatus() {
    return this->lastStatus;
}

void OS::setLastStatus(Status status) {
    this->lastStatus = status;
}

void* _cdecl operator new (SIZE size, Pool* pool) {
	if (pool == NULL) {
		os->setLastStatus(NullPointer);
		return NULL;
	}
	os->setLastStatus(Success);
	return pool->allocate(size);
}

void* _cdecl operator new[](SIZE size, Pool* pool) {
	if (pool == NULL) {
		os->setLastStatus(NullPointer);
		return NULL;
	}
	os->setLastStatus(Success);
	return pool->allocate(size);
}

