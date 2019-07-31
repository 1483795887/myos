#pragma once
#include "../Types.h"
#include "CListEntry.h"

class CList{
public:
	CList();
	ULONG getCount();
	void insertHead(CListEntry* entry);
	void remove(CListEntry* entry);
	CListEntry *getHead();
	CListEntry *getFirst();
private:
	ULONG count;
	CListEntry head;
};

