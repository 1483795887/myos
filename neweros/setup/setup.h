#pragma once

#include <Types.h>

#define GraphicInfo		0x00002200
#define MemoryInfo		0x00002300
#define DotFontMap		0x00004000
#define KernelTempBase  0x00050000
#define KernelImageBase 0xc0100000

typedef struct {
	DWORD baseAddrLow;
	DWORD baseAddrHigh;
	DWORD lengthLow;
	DWORD lengthHigh;
	DWORD type;
}MemLayout;

typedef struct {
	DWORD rangeNums;
	MemLayout ranges[32];
}MemInfo;