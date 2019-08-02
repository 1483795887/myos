#pragma once
#include "Pool.h"

class NaivePool : public Pool {
public:
    virtual PBYTE allocate(SIZE size);

	NaivePool(PBYTE start, SIZE size);

private:
	PBYTE nextAddress;
	SIZE remainedSize;
};