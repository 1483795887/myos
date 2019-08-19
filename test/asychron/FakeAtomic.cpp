#include "pch.h"
#include <synchron/Atomic.h>

void atomicAdd(int* val, int par) {
	*val += par;
}