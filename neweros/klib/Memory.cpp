#include <klib/Memory.h>

void _cdecl memcpy(PBYTE from, PBYTE to, SIZE size) {
	PBYTE pFrom = (PBYTE)from;
	PBYTE pTo = (PBYTE)to;
    for (int i = 0; i < size; i++) {
        *pTo = *pFrom;
		pTo++;
		pFrom++;
    }
}

void _cdecl memset(PBYTE dest, BYTE val, SIZE size) {
	PBYTE pDest = (PBYTE)dest;
	for (int i = 0; i < size; i++) {
		*pDest = (BYTE)val;
		pDest++;
	}
}