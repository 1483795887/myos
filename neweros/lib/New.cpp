#include <mm/Pool.h>

void* _cdecl operator new(SIZE size, Pool* pool) {
	if (pool == NULL)
		return NULL;
	return pool->allocate(size);
}