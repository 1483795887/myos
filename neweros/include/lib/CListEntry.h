#pragma once
#include "../types.h"

class CListEntry {
public:
	CListEntry() {
		prev = this;
		next = this;
	}
    CListEntry* getPrev() {
        return prev;
    }
    CListEntry* getNext() {
		return next;
    }
	void setPrev(CListEntry* prev) {
		this->prev = prev;
	}
	void setNext(CListEntry* next) {
		this->next = next;
	}
private:
    CListEntry* prev;
    CListEntry* next;
};