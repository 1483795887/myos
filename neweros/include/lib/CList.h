#pragma once
#include "../Types.h"
#include "CListEntry.h"

class CList{
public:
	CList();
	ULONG getCount();
	void insertHead(CListEntry* entry);
	void insertAfter(CListEntry* prev, CListEntry* entry);
	void insertBefore(CListEntry* next, CListEntry* entry);
	void remove(CListEntry* entry);
	CListEntry *getHead();
	CListEntry *getFirst();
	BOOL isEmpty();
private:
	ULONG count;
	CListEntry head;
};

