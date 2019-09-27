#include "pch.h"
#include <global/OS.h>
#include <mm/PageMapper.h>
#include <mm/BucketPool.h>
#include "FakePageAllocator.h"

class BucketPoolTest :public testing::Test {
public:
	virtual void SetUp() {
		allocator = new FakePageAllocator();
		pool = new BucketPool;
		pool->setAllocator(allocator);
		os->setLastStatus(StatusSuccess);
	}
	virtual void TearDown() {
		delete pool;
		delete allocator;
	}

	FakePageAllocator* allocator;
	BucketPool* pool;
};

TEST_F(BucketPoolTest, noPageForDirWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(0);
	pool->allocate(16);
	EXPECT_EQ(pool->getStatus(), StatusPoolNotEnough);
}

TEST_F(BucketPoolTest, sizeTooBigWhenAllocateThenReturnStatus) {
	pool->allocate(PAGE_SIZE + 1);
	EXPECT_EQ(pool->getStatus(), StatusSizeTooBig);
}

TEST_F(BucketPoolTest, pageForDirNotEnoughWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(MAX_POOL_PAGES - 1);
	pool->allocate(16);
	EXPECT_EQ(pool->getStatus(), StatusPoolNotEnough);
}

TEST_F(BucketPoolTest, pageForPoolNotEnoughWhenAllocateThenReturnStatus) {
	allocator->setRemainPages(MAX_POOL_PAGES);
	pool->allocate(16);
	EXPECT_EQ(pool->getStatus(), StatusPoolNotEnough);
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
	EXPECT_EQ(pool->getStatus(), StatusPoolNotEnough);
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
	EXPECT_EQ(pool->getStatus(), StatusPoolNotEnough);
}

TEST_F(BucketPoolTest, notEverAllocatedWhenCheckInPoolThenFalse) {
	EXPECT_FALSE(pool->isInPool(0x0));
}

TEST_F(BucketPoolTest, notSamePageWhenCheckInPoolThenFalse) {
	PBYTE ptr = pool->allocate(16);
	EXPECT_FALSE(pool->isInPool(0x0));
}

TEST_F(BucketPoolTest, onlyOnePoolWhenCheckInPoolThenTrue) {
	PBYTE ptr = pool->allocate(16);
	EXPECT_TRUE(pool->isInPool(ptr));
}

TEST_F(BucketPoolTest, multiplyPoolsOfSameSizeInOnePageWhenCheckInPoolThenTrue) {
	PBYTE ptr;
	for (int i = 0; i < 16; i++) {
		ptr = pool->allocate(16);
	}
	EXPECT_TRUE(pool->isInPool(ptr));
}

TEST_F(BucketPoolTest, multiplyPoolsOfDifferentSizeWhenCheckInPoolThenTrue) {
	PBYTE ptr;
	for (int i = 0; i < MAX_POOL_ORDER; i++) {
		ptr = pool->allocate(16 << i);
		ptr = pool->allocate(16 << i);
	}
	EXPECT_TRUE(pool->isInPool(ptr));
}

TEST_F(BucketPoolTest, multiplyPoolsOfSameSizeInMoreThanOnePageWhenCheckInPoolThenTrue) {
	PBYTE ptr;
	for (int i = 0; i < PAGE_SIZE / 16; i++) {
		ptr = pool->allocate(16);
	}

	EXPECT_TRUE(pool->isInPool(pool->allocate(16)));
}

TEST_F(BucketPoolTest, fullPoolWhenFreeThenNotFull) {
	allocator->setRemainPages(MAX_POOL_PAGES + 1);
	PBYTE ptr = NULL;
	for (int i = 0; i < PAGE_SIZE / MIN_POOL_SIZE; i++) {
		ptr = pool->allocate(MIN_POOL_SIZE);
	}
	pool->free(ptr);
	EXPECT_NE(pool->allocate(MIN_POOL_SIZE), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, fullPoolFreeMemNotInPageWhenFreeThenStillFull) {
	allocator->setRemainPages(MAX_POOL_PAGES + 1);
	PBYTE ptr = NULL;
	for (int i = 0; i < PAGE_SIZE / MIN_POOL_SIZE; i++) {
		ptr = pool->allocate(MIN_POOL_SIZE);
	}
	pool->free(0x0);
	EXPECT_EQ(pool->allocate(MIN_POOL_SIZE), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, fullPoolFreeMemInPageButNotInPoolWhenFreeThenStillFull) {
	allocator->setRemainPages(MAX_POOL_PAGES + 1);
	PBYTE ptr = NULL;
	for (int i = 0; i < PAGE_SIZE / MIN_POOL_SIZE; i++) {
		ptr = pool->allocate(MIN_POOL_SIZE);
	}
	pool->free(ptr);
	pool->free(ptr);
	pool->allocate(MIN_POOL_SIZE);
	pool->allocate(MIN_POOL_SIZE);
	EXPECT_EQ(pool->allocate(MIN_POOL_SIZE), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, freeLastPoolInPageWhenFreeThenEntryFree) {
	allocator->setRemainPages(MAX_POOL_PAGES + 1);
	PBYTE ptr = NULL;
	ptr = pool->allocate(MIN_POOL_SIZE);
	pool->free(ptr);
	EXPECT_NE(pool->allocate(MIN_POOL_SIZE * 2), (PBYTE)NULL);
}

TEST_F(BucketPoolTest, freeLastPoolInPageWhenFreeThenCanStillAllocateSameSize) {
	PBYTE ptr = NULL;

	pool->allocate(PAGE_SIZE);
	ptr = pool->allocate(PAGE_SIZE);
	pool->allocate(PAGE_SIZE);
	pool->free(ptr);
	EXPECT_NE(pool->allocate(PAGE_SIZE), (PBYTE)NULL);
}