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

typedef enum {
	Kernel = 0,
	User = 3
} Privilege;

class GlobalDescriptorTable {
public:
	void setTable(PBYTE table) {
		this->table = table;
	}
	void setDescriptor(ULONG n, PBYTE start, ULONG limit, DescriptorType type, Privilege privilege);
private:
	PBYTE table;
};

typedef enum {
	Interrupt,
	Trap
} InterruptType;

class InterruptVectorTable {
public:
	void setTable(PBYTE table) {
		this->table = table;
	}
	void setInterruptVector(IntType vec, InterruptRoutine routine, InterruptType type, Privilege privilege);
private:
	PBYTE table;
};