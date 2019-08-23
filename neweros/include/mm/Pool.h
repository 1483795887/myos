#pragma once
#include "../Types.h"

class Pool {
public:
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
};