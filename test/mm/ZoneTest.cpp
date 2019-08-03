#include "pch.h"
#include <mm/zone.h>
#include "FakePhysicalPageAllocator.h"
#include <mm/Page.h>
#include <mm/FakePool.h>
#include <global/OS.h>


class ZoneTest :public testing::Test {
public:
	virtual void SetUp() {
		fakePool = new FakePool(1024);
		os->pool = fakePool;
	}

	virtual void TearDown() {
		delete fakePool;
	}

	FakePool* fakePool;
};
