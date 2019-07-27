#include <lib/CList.h>

CList::CList() {
    count = 0;
	head.next = &head;
	head.prev = &head;
}

ULONG CList::getCount() {
    return count;
}

void CList::insertHead(CListEntry* entry) {
    if (entry != NULL) {
		CListEntry* oldNext = head.next;
		head.next = entry;
		entry->next = oldNext;
		entry->prev = &head;
		oldNext->prev = entry;
		count++;
    }
}

void CList::remove(CListEntry * entry)
{
	BOOL find = FALSE;
	if (entry != NULL && count > 0) {
		for (CListEntry* begin = head.next; begin != &head; begin = begin->next) {
			if (begin == entry) {
				find = TRUE;
				break;
			}
		}
		if (find) { 
			CListEntry* oldNext = entry->next;
			entry->prev->next = oldNext;
			oldNext->prev = entry->prev;
			count--;
		}
	}
}

CListEntry* CList::getHead() {
	return &head;
}

CListEntry * CList::getFirst()
{
	return head.next;
}
