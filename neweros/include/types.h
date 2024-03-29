#pragma once

#define NULL 0
#define FAILED -1

#define K (1 << 10)
#define M (1 << 20)
#define G (1 << 30)

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;

typedef unsigned char* PBYTE;

typedef DWORD SIZE;
typedef DWORD ULONG;

typedef enum {
    FALSE = 0, TRUE = 1
} BOOL;

typedef enum {
	Kernel = 0,
	User = 3
} Privilege;

inline BOOL NOT(BOOL val) {
    if (val)
        return FALSE;
    else
        return TRUE;
}

inline ULONG ulAlign(ULONG ul, ULONG size, BOOL upper) {
    ULONG alignedAddress = ul & (-size);
    if (alignedAddress < ul && upper)
        alignedAddress += size;
    return alignedAddress;
}

inline BOOL checkAligned(ULONG ul, ULONG size) {
    return (BOOL)(ul % size == 0);
}