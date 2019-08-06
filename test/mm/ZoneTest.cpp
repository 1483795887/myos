#include "pch.h"
#include <mm/zone.h>
#include "FakePhysicalPageAllocator.h"
#include <mm/Page.h>
#include <mm/FakePool.h>
#include <mm/NaivePool.h>
#include <mm/FakePhysicalPageAllocator.h>
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

TEST_F(ZoneTest, orderNotInRangeWhenInitFreeAreaThenError) {
	FreeArea freeArea;
	EXPECT_EQ(freeArea.init(1111, 10), ValueNotInRange);
}

TEST_F(ZoneTest, emptyZoneWhenGetFreePagesThenZero) {
	Zone zone;
	FakePhysicalPageAllocator allocator;
	zone.init(allocator.allocPages(NULL, 0), PAGE_SIZE);
	EXPECT_EQ(zone.getFreePages(), 0);
}