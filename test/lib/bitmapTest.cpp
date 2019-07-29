#include "pch.h"
#include <types.h>
#include <lib/bitmap.h>

class BitmapTest : public testing::Test {
public:
	virtual void SetUp() {
		map = (PBYTE)malloc(100);
		memset(map, 0, 100);
		bitmap.init(map, 100);
	}
	virtual void TearDown() {

	}
	Bitmap bitmap;
	PBYTE map;
};

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