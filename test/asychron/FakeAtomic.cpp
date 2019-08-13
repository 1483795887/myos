#include "pch.h"
#include "FakeAtomic.h"

static void(*oldAdd)(int*, int);

aAdd atomicAdd = fakeAdd;

void fakeAdd(int* val, int par) {
	*val += par;
}