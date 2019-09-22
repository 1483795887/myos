#pragma once
#include "../types.h"
#include <mm/PageMapper.h>

class CPU {
public:
	static void openPageMode();
	static void setGlobalDescriptorTable(PBYTE table);
	static void setInterruptVectorTable(PBYTE table);
};