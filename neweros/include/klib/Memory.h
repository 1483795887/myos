#pragma once
#include <../Types.h>

extern "C" void _cdecl memcpy(PBYTE from, PBYTE to, SIZE size);
extern "C" void _cdecl memset(PBYTE dest, BYTE val, SIZE size);