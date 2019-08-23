#include <lib/CList.h>

CList::CList() {
    count = 0;
}

ULONG CList::getCount() {
    return count;
}

void CList::insertHead(CListEntry* entry) {
    if (entry != NULL) {
		head.insertNext(entry);
        count++;
    }
}

void CList::remove(CListEntry* entry) {
    BOOL find = FALSE;
    if (entry != NULL && count > 0) {
        for (CListEntry* begin = head.getNext(); begin != &head; begin = begin->getNext()) {
            if (begin == entry) {
                find = TRUE;
                break;
            }
        }
        if (find) {
			entry->removeThis();
            count--;
        }
    }
}

CListEntry* CList::getHead() {
    return &head;
}

CListEntry* CList::getFirst() {
    return head.getNext();
}

BOOL CList::isEmpty() {
    return (BOOL)(getFirst() == getHead());
}
