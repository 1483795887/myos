#include <synchron/Atomic.h>

extern "C" void _cdecl aAdd(int*, int par);

void atomicAdd(int* val, int par) {
	aAdd(val, par);
}