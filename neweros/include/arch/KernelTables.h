#pragma once

#include "../interrupt/Interrrupt.h"
#include "../types.h"

enum {
	Dummpy = 0,
	KernelCode = 1,
	KernelData = 2,
	UserCode = 3,
	UserData = 4
};

typedef enum {
	Code,
	Data,
	TSS
} DescriptorType;

class GlobalDescriptorTable {
public:
	GlobalDescriptorTable(PBYTE table) {
		this->table = table;
	}
	void setDescriptor(ULONG n, PBYTE start, ULONG limit, DescriptorType type, Privilege privilege);
private:
	PBYTE table;
};