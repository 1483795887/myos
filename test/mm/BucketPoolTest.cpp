#include "pch.h"
#include <global/OS.h>
#include <mm/PhysicalPage.h>
#include <mm/BucketPool.h>
#include "FakePhysicalPageAllocator.h"

class BucketPoolTest :public testing::Test {
public:
	virtual void SetUp() {
		allocator = new FakePhysicalPageAllocator();
		pool = new BucketPool;
		pool->setAllocator(allocator);
		os->setLastStatus(Success);
	}
	virtual void TearDown() {
		delete pool;
		delete allocator;
	}

	FakePhysicalPageAllocator* allocator;
	BucketPool* pool;
};

TEST_F(BucketPoolTest, noPageForDirWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(0);
	pool->allocate(16);
	EXPECT_EQ(os->getLastStatus(), PoolNotEnough);
}

TEST_F(BucketPoolTest, sizeTooBigWhenAllocateThenReturnStatus) {
	pool->allocate(PAGE_SIZE + 1);
	EXPECT_EQ(os->getLastStatus(), SizeTooBig);
}

TEST_F(BucketPoolTest, pageForDirNotEnoughWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(MAX_POOL_PAGES - 1);
	pool->allocate(16);
	EXPECT_EQ(os->getLastStatus(), PoolNotEnough);
}

TEST_F(BucketPoolTest, pageForPoolNotEnoughWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(MAX_POOL_PAGES);
	pool->allocate(16);
	EXPECT_EQ(os->getLastStatus(), PoolNotEnough);
}

TEST_F(BucketPoolTest, sixteenWhenAllocateThenReturnNotZero) {
	EXPECT_NE(pool->allocate(16), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, onePageWhenAllocateThenReturnNotZero) {
	EXPECT_NE(pool->allocate(PAGE_SIZE), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, lackOfOneMorePageForPoolWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(MAX_POOL_PAGES + 1);
	for (int i = 0; i < PAGE_SIZE / 16; i++) {
		pool->allocate(16);
	}
	pool->allocate(16);
	EXPECT_EQ(os->getLastStatus(), PoolNotEnough);
}

TEST_F(BucketPoolTest, noLackOfOneMorePageForPoolWhenAllocateThenReturnNotZero) {
	for (int i = 0; i < PAGE_SIZE / 16; i++) {
		pool->allocate(16);
	}
	EXPECT_NE(pool->allocate(16), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, useUpOfDirMemoryWhenAllocateThenReturnStatus) {
	for (int i = 0; i < PAGE_SIZE * MAX_POOL_PAGES / sizeof(BucketEntry); i++) {
		pool->allocate(PAGE_SIZE);
	}
	pool->allocate(PAGE_SIZE);
	EXPECT_EQ(os->getLastStatus(), PoolNotEnough);
}