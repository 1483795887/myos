#include <lib/Memory.h>

void _cdecl memcpy(void* dst, const void* src, SIZE size) {
	PBYTE pFrom = (PBYTE)src;
	PBYTE pTo = (PBYTE)dst;
    for (int i = 0; i < size; i++) {
        *pTo = *pFrom;
		pTo++;
		pFrom++;
    }
}

void _cdecl memset(void* dest, int val, SIZE size) {
	PBYTE pDest = (PBYTE)dest;
	for (int i = 0; i < size; i++) {
		*pDest = (BYTE)val;
		pDest++;
	}
}