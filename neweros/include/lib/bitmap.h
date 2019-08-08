#pragma once

#include "../Types.h"
#include "../Status.h"

class Bitmap {
public:
	Status init(ULONG maxno);
	BOOL checkBit(ULONG no);
	void setBit(ULONG no);
	void notBit(ULONG no);
	void revBit(ULONG no);
private:
	ULONG maxno;
	PBYTE map;
};