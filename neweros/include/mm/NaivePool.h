#pragma once
#include "Pool.h"

class NaivePool : public Pool {
public:
    virtual PBYTE allocate(SIZE size);

	NaivePool(PBYTE start, SIZE size);

	void setStart(PBYTE start) {
		this->nextAddress = start;
	}

	void setSize(SIZE size) {
		remainedSize = size;
	}

	virtual ~NaivePool() {

	}

private:
	PBYTE nextAddress;
	SIZE remainedSize;
};