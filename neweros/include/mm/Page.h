#pragma once
#include <Types.h>
#include <lib/CListEntry.h>

class Page : public CListEntry {
public:
    PBYTE getAddress() {
        return address;
    }
	void setAddress(PBYTE address) {
		this->address = address;
	}
private:
	PBYTE address;
};