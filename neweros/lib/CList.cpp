#include <lib/CList.h>

CList::CList() {
    count = 0;
}

ULONG CList::getCount() {
    return count;
}

void CList::insertHead(CListEntry* entry) {
    if (entry != NULL) {
		CListEntry* oldNext = head.getNext();
		head.setNext(entry);
		entry->setNext(oldNext);
		entry->setPrev(&head);
		oldNext->setPrev(entry);
		count++;
    }
}

void CList::remove(CListEntry * entry)
{
	BOOL find = FALSE;
	if (entry != NULL && count > 0) {
		for (CListEntry* begin = head.getNext(); begin != &head; begin = begin->getNext()) {
			if (begin == entry) {
				find = TRUE;
				break;
			}
		}
		if (find) { 
			CListEntry* oldPrev = entry->getPrev();
			CListEntry* oldNext = entry->getNext();
			oldPrev->setNext(oldNext);
			oldNext->setPrev(oldPrev);
			count--;
		}
	}
}

CListEntry* CList::getHead() {
	return &head;
}

CListEntry * CList::getFirst()
{
	return head.getNext();
}
