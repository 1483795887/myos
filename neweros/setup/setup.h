#pragma once

#include <Types.h>

#define KernelImageBase 0xc0100000
#define KernelTempBase  0x00050000
#define MemoryInfo		0x00023000

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