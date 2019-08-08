#include "pch.h"
#include <types.h>
#include <lib/bitmap.h>
#include <global/OS.h>
#include "../mm/FakePool.h"
#include <mm/NaivePool.h>

class BitmapTest : public testing::Test {
public:
    virtual void SetUp() {

        fakePool = new FakePool(1024);
        os->pool = fakePool;
        bitmap.init(100);
    }
    virtual void TearDown() {
        delete fakePool;
    }
    Bitmap bitmap;

    FakePool* fakePool;

};

TEST_F(BitmapTest, noPoolWhenCheckBitThenError) {
    Bitmap bitmap;
    os->pool = NULL;
    EXPECT_EQ(bitmap.init(100), NullPointer);
}

TEST_F(BitmapTest, poolNotEnoughWhenCheckBitThenError) {
    Bitmap bitmap;
    os->pool = new FakePool(10);
    EXPECT_EQ(bitmap.init(100), PoolNotEnough);
    delete os->pool;
}

TEST_F(BitmapTest, emptyBitmapWhenCheckBitThenFalse) {
    EXPECT_EQ(bitmap.checkBit(0), FALSE);
}

TEST_F(BitmapTest, setFirstBitWhenCheckBitThenTrue) {
    bitmap.setBit(0);
    EXPECT_EQ(bitmap.checkBit(0), TRUE);
}

TEST_F(BitmapTest, setFirstBitAndNotItWhenCheckBitThenFalse) {
    bitmap.setBit(0);
    bitmap.notBit(0);
    EXPECT_EQ(bitmap.checkBit(0), FALSE);
}

TEST_F(BitmapTest, setFirstBitWhenCheckBitThenNotChangeOther) {
    bitmap.setBit(0);
    EXPECT_EQ(bitmap.checkBit(1), FALSE);
}

TEST_F(BitmapTest, setBitTwiceThenOn) {
    bitmap.setBit(0);
    bitmap.setBit(0);
    EXPECT_EQ(bitmap.checkBit(0), TRUE);
}

TEST_F(BitmapTest, setNinthBitWhenCheckBitThenTrue) {
    bitmap.setBit(8);
    EXPECT_EQ(bitmap.checkBit(8), TRUE);
}

TEST_F(BitmapTest, setFirstBitAndNotItWhenCheckBitThenNotChangeOther) {
    bitmap.setBit(0);
    bitmap.notBit(0);
    EXPECT_EQ(bitmap.checkBit(1), FALSE);
}

TEST_F(BitmapTest, notBitTwiceThenOff) {
    bitmap.notBit(0);
    bitmap.notBit(0);
    EXPECT_EQ(bitmap.checkBit(0), FALSE);
}