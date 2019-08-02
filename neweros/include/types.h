#pragma once

#define NULL 0
#define FAILED -1

#define K (1 << 10)
#define M (1 << 20)
#define G (1 << 30)

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;

typedef unsigned char* PBYTE;

typedef DWORD SIZE;
typedef DWORD ULONG;

typedef enum {
	FALSE = 0 ,TRUE = 1
}BOOL;

inline ULONG addressAlign(ULONG address, ULONG size, BOOL upper) {
	ULONG alignedAddress = address & (-size);
	if (alignedAddress < address && upper)
		alignedAddress += size;
	return alignedAddress;
}