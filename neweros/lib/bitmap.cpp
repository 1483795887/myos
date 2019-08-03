#include <global/OS.h>
#include <lib/Bitmap.h>


Bitmap::Bitmap(ULONG maxNo)
{
	maxNo = ulAlign(maxNo, 8, TRUE);

	//Bitmap* bitmap = New Bitmap(10);
	map = New BYTE[maxNo];
}

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
