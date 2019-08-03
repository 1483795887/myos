#pragma once

#include "../Types.h"

class Bitmap {
public:
	Bitmap(){}
	Bitmap(ULONG maxno);
	BOOL checkBit(ULONG no);
	void setBit(ULONG no);
	void notBit(ULONG no);
private:
	ULONG maxno;
	PBYTE map;
};