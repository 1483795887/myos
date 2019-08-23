#pragma once
#include "../Types.h"

class CListEntry {
public:
	CListEntry(); 
	CListEntry* getPrev() {
        return prev;
    }
	CListEntry* getNext() {
		return next;
    }
	void setPrev(CListEntry* prev){
		this->prev = prev;
	}
	void setNext(CListEntry* next) {
		this->next = next;
	}
	void setAddress(PBYTE ptr){
		prev = (CListEntry*)ptr;
		next = (CListEntry*)ptr;
	}
	void insertNext(CListEntry* entry);
	void removeThis(); 
private:
    CListEntry* prev;
    CListEntry* next;
};