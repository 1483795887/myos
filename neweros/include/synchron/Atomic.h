#pragma once

#include "../Types.h"

typedef void(_cdecl *aAdd)(int*, int);

extern "C" aAdd atomicAdd;

class Atomic {
public:
	Atomic() {
		count = 0;
	}
	void add(int val) {
		atomicAdd(&count, val);
	}
	void sub(int val) {
		atomicAdd(&count, -val);
	}
	void set(int val) {
		count = val;
	}
	int getCount() {
		return count;
	}
private:
	int count;
};