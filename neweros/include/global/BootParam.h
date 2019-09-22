#pragma once
#include <Types.h>

class BootParams {
public:
	ULONG codeStart;	//虚拟地址
	SIZE  codeSize;
	ULONG rdataStart;
	SIZE  rdataSize;
	ULONG dataStart;
	SIZE  dataSize;

	ULONG kernelStart;
	ULONG memoryEnd;	//物理地址

	PD pd;

	PBYTE pool;
	SIZE poolSize;

	PBYTE graphicInfo;
	PBYTE font;
};