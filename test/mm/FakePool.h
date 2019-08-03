#pragma once
#include <mm/Pool.h>


class FakePool : public Pool {
public:
	virtual PBYTE allocate(SIZE size);

	FakePool(SIZE size);

	virtual ~FakePool();
private:
	PBYTE pool;
	PBYTE current;
	ULONG remainSize;
};