#pragma once

#include "../types.h"

class Bitmap {
public:
	BOOL checkBit(ULONG no);
	void setBit(ULONG no);
	void notBit(ULONG no);
	void init(PBYTE map, ULONG maxno);
private:
	ULONG maxno;
	PBYTE map;
};