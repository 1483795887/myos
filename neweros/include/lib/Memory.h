#pragma once
#include <../Types.h>

extern "C" void _cdecl memcpy(void* dest, const void* src, SIZE size);
extern "C" void _cdecl memset(void* dest, int val, SIZE size);