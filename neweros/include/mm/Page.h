#pragma once
#include <Types.h>
#include <lib/CListEntry.h>

class Page : public CListEntry {
public:
    PBYTE getData() {
        return address;
    }
    void setData(PBYTE data) {
        this->address = data;
    }
private:
    PBYTE address;
};