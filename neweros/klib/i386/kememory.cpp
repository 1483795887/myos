#include <klib/Kememory.h>

void KeMemCopy(PBYTE from, PBYTE to, DWORD size) {
    for (int i = 0; i < size; i++) {
        *to = *from;
        to++;
        from++;
    }
}