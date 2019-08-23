#include <lib/CListEntry.h>

CListEntry::CListEntry() {
    prev = this;
    next = this;
}

void CListEntry::insertNext(CListEntry* entry) {
	if (entry != NULL) {
		CListEntry* oldNext = getNext();
		setNext(entry);
		entry->setNext(oldNext);
		entry->setPrev(this);
		oldNext->setPrev(entry);
	}
}

void CListEntry::removeThis() {
	if (prev != this) {
		CListEntry* oldPrev = prev;
		CListEntry* oldNext = next;
		oldPrev->setNext(oldNext);
		oldNext->setPrev(oldPrev);
	}
}
