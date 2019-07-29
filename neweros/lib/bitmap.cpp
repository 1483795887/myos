#include <lib/bitmap.h>

BOOL Bitmap::checkBit(ULONG no) {
    ULONG byte = no / 8;
    BYTE bit = no % 8;
    return (BOOL)(map[byte] & (1 << bit));
}

void Bitmap::init(PBYTE map, ULONG maxno) {
    this->map = map;
    this->maxno = maxno;
}

void Bitmap::setBit(ULONG no) {
	ULONG byte = no / 8;
	BYTE bit = no % 8;
	map[byte] |= (1 << bit);
}

void Bitmap::notBit(ULONG no) {
	ULONG byte = no / 8;
	BYTE bit = no % 8;
	map[byte] &= ~(1 << bit);
}
