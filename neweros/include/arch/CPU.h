#pragma once
#include "../types.h"

class CPU {
public:
	static void openPageMode();
	static void setGlobalDescriptorTable(PBYTE table);
	static void setInterruptVectorTable(PBYTE table);
};