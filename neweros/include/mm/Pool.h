#pragma once
#include "../Types.h"

class Pool {
public:
    virtual PBYTE allocate(SIZE size) {
        return NULL;
    };
    virtual ~Pool() {

    }
};