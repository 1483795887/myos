#pragma once
#include <../Types.h>

extern "C" void _cdecl memcpy(void* dest, const void* src, SIZE size);//仅限内核使用
extern "C" void _cdecl memset(void* dest, int val, SIZE size);