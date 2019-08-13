#pragma once
#include <Types.h>
#include <lib/CListEntry.h>
#include <synchron/Atomic.h>

class Page : public CListEntry {
public:
    PBYTE getAddress() {
        return address;
    }
    void setAddress(PBYTE address) {
        this->address = address;
    }
    void incCount() {
        count.add(1);
    }
    void decCount() {
        count.sub(1);
    }
	void setCount(int val) {
		count.set(val);
	}
    int getCount() {
        return count.getCount();
    }
private:
    PBYTE address;
    Atomic count;
};