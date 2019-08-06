#include <global/OS.h>
#include <lib/Bitmap.h>

BOOL Bitmap::checkBit(ULONG no) {
    if (no > maxno)
        return FALSE;
    ULONG byte = no / 8;
    BYTE bit = no % 8;
    return (BOOL)(map[byte] & (1 << bit));
}

Status Bitmap::init(ULONG maxNo) {
    this->maxno = ulAlign(maxNo, 8, TRUE);
    map = New BYTE[maxNo];
    return os->getLastStatus();
}

void Bitmap::setBit(ULONG no) {
    if (no <= maxno) {
        ULONG byte = no / 8;
        BYTE bit = no % 8;
        map[byte] |= (1 << bit);
    }
}

void Bitmap::notBit(ULONG no) {
    if (no <= maxno) {
        ULONG byte = no / 8;
        BYTE bit = no % 8;
        map[byte] &= ~(1 << bit);
    }
}
