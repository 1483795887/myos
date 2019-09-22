#pragma once
#include "../Types.h"
#include "../Status.h"

class Pool {
public:
    Pool() {
        status = StatusSuccess;
    }
    virtual PBYTE allocate(SIZE size) {
        return NULL;
    };
    virtual void free(PBYTE addr) {

    }
    virtual ~Pool() {

    }
    virtual BOOL isInPool(PBYTE ptr) {
        return FALSE;
    }
    Status getStatus() {
        return status;
    }
protected:
    Status status;
};