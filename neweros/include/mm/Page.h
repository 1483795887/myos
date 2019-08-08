#pragma once
#include <Types.h>
#include <lib/CListEntry.h>

class Page : public CListEntry {
public:
    PBYTE getAddress() {
        return (PBYTE)this;
    }
	ULONG getOrder() {
		return order;
	}
	void setOrder(ULONG order) {
		this->order = order;
	}
private:
	ULONG order;
};