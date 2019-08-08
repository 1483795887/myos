#include "pch.h"
#include <mm/NaivePool.h>
#include "FakePool.h"

class NaivePoolTest : public testing::Test {
public:
    virtual void SetUp() {
        fakePool = new FakePool(1024);
        mPool = fakePool->allocate(1024);
    }

    virtual void TearDown() {
        delete fakePool;
    }

    FakePool* fakePool;
    PBYTE mPool;
};

TEST_F(NaivePoolTest, sizeTooBigWhenAllocateThenReturnNull) {
    NaivePool pool(mPool, 4 * G - 1);
    EXPECT_EQ(pool.allocate(4 * G - 1), (PBYTE)NULL);
}

TEST_F(NaivePoolTest, normalWhenAllocateThenResultNotNull) {
    NaivePool pool(mPool, 10);
    EXPECT_NE(pool.allocate(5), (PBYTE)NULL);
}

TEST_F(NaivePoolTest, allocateTwoAllocateThenMemoryContinue) {
    NaivePool pool(mPool, 10);
    PBYTE pool1 = pool.allocate(4);
    PBYTE pool2 = pool.allocate(4);
    EXPECT_EQ((ULONG)(pool1 + 4), (ULONG)pool2);
}

TEST_F(NaivePoolTest, onceSizeOverPoolLimitAllocateThenResultNull) {
    NaivePool pool(mPool, 10);
    EXPECT_EQ(pool.allocate(50), (PBYTE)NULL);
}

TEST_F(NaivePoolTest, useExactOverAllocateThenResultNotNull) {
    NaivePool pool(mPool, 10);
    pool.allocate(4);
    pool.allocate(2);
    EXPECT_NE(pool.allocate(2), (PBYTE)NULL);
}

TEST_F(NaivePoolTest, multiplySizeOverWhenAllocateThenResultNull) {
    NaivePool pool(mPool, 10);
    pool.allocate(4);
    pool.allocate(4);
    EXPECT_EQ(pool.allocate(5), (PBYTE)NULL);
}