#pragma once
#include "../types.h"
#include <mm/PageMapper.h>

extern "C" void _cdecl openPageMode();
extern "C" void _cdecl setGlobalDescriptorTable(PBYTE table);
extern "C" void _cdecl setInterruptVectorTable(PBYTE table);
extern "C" void _cdecl halt();