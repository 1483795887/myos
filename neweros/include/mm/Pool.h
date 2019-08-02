#pragma once
#include "../Types.h"

class Pool {
public:
    virtual PBYTE allocate(SIZE size) = 0;
    virtual ~Pool() {

    }
};