#include "pch.h"
#include <global/OS.h>
#include <mm/FakePool.h>
#include <mm/FakePhysicalPageAllocator.h>
#include <mm/PhysicalPageAllocatorImpl.h>

class PhysicalPageAllocatorImplTest :public testing::Test {
public:
	virtual void SetUp() {
		fakePool = new FakePool(2048);
		os->pool = fakePool;
		os->setLastStatus(Success);
		allocator = new FakePhysicalPageAllocator();
	}

	virtual void TearDown() {
		delete fakePool;
		delete allocator;
	}

	FakePool* fakePool;
	PhysicalPageAllocator* allocator;
};

/*TEST_F(PhysicalPageAllocatorImplTest, orderNotInRangeWhenAllocatePagesThenReturnNULL) {
	PBYTE address = allocator->allocPages(NULL, 0);
	Zone zone;
	zone.init(address, PAGE_SIZE);

	PhysicalPageAllocatorImpl impl;
	EXPECT_EQ((ULONG)impl.allocPages(&zone, 100), NULL);
}

TEST_F(PhysicalPageAllocatorImplTest, onePageWhenAllocatePagesThenAddressSame) {
	PBYTE address = allocator->allocPages(NULL, 0);
	Zone zone;
	zone.init(address, PAGE_SIZE);

	PhysicalPageAllocatorImpl impl;
	EXPECT_EQ(impl.allocPages(&zone, 0), address);
}

TEST_F(PhysicalPageAllocatorImplTest, onePageFromNoPageWhenAllocatePagesThenAddressNULL) {
	PBYTE address = allocator->allocPages(NULL, 0);
	Zone zone;
	zone.init(address, PAGE_SIZE - 1);

	PhysicalPageAllocatorImpl impl;
	EXPECT_EQ(impl.allocPages(&zone, 0), (PBYTE)NULL);
}

TEST_F(PhysicalPageAllocatorImplTest, onePageFromOnePageOrderTwoWhenAllocatePagesThenAddressSame) {
	PBYTE address = allocator->allocPages(NULL, 1);
	Zone zone;
	zone.init(address, PAGE_SIZE * 2);

	PhysicalPageAllocatorImpl impl;
	EXPECT_EQ(impl.allocPages(&zone, 0), address);
}

TEST_F(PhysicalPageAllocatorImplTest, twoPageFromOnePageOrderTwoWhenAllocatePagesThenSecondAddressSame) {
	PBYTE address = allocator->allocPages(NULL, 1);
	Zone zone;
	zone.init(address, PAGE_SIZE * 2);

	PhysicalPageAllocatorImpl impl;

	impl.allocPages(&zone, 0);

	EXPECT_EQ(impl.allocPages(&zone, 0), (PBYTE)address + PAGE_SIZE);
}*/