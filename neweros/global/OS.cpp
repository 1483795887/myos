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
        os->setLastStatus(StatusNullPointer);
        return NULL;
    }
    os->setLastStatus(StatusSuccess);
    return pool->allocate(size);
}

void* _cdecl operator new[](SIZE size, Pool* pool) {
    if (pool == NULL) {
        os->setLastStatus(StatusNullPointer);
        return NULL;
    }
    os->setLastStatus(StatusSuccess);
    return pool->allocate(size);
}

/*编译器会给纯虚类也填一个_purecall,平时在CRT中会提供但这里没有,便提供一个*/
extern "C" int _cdecl _purecall() {
	return 0;
}