#pragma once
#include "../types.h"

class CListEntry {
public:
	ULONG data;
	CListEntry *prev;
	CListEntry *next;
};