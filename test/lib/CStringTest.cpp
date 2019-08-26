#include "pch.h"
#include <global/OS.h>
#include <mm/FakePool.h>
#include <mm/FakePhysicalPageAllocator.h>
#include <lib/CString.h>


class CStringTest : public testing::Test {
public:
    virtual void SetUp() {
		fakePool = new FakePool(4096);
		os->pool = fakePool;
		os->setLastStatus(Success);
    }
	virtual void TearDown() {
		delete fakePool;
	}

	FakePool* fakePool;
};

TEST_F(CStringTest, stringShortThanMinMaxLenWhenInitThenStrSame) {
	char testStr[] = "test";
	CString str(testStr);
	EXPECT_STREQ(str.getBuf(), testStr);
}

TEST_F(CStringTest, stringLongerThanMinMaxLenWhenInitThenStrSame) {
	char testStr[] = "testtesttesttesttest";
	CString str(testStr);
	EXPECT_STREQ(str.getBuf(), testStr);
}

TEST_F(CStringTest, stringNullWhenInitThenStrEmpty) {
	CString str(NULL);
	EXPECT_STREQ(str.getBuf(), "");
}

TEST_F(CStringTest, stringEmptyWhenInitThenStrEmpty) {
	char testStr[] = "";
	CString str(testStr);
	EXPECT_STREQ(str.getBuf(), testStr);
}

TEST_F(CStringTest, normalStrWhenGetLenThenLenRight) {
	char testStr[] = "test";
	CString str(testStr);
	EXPECT_EQ(str.getLen(), 4);
}

TEST_F(CStringTest, nullStrWhenGetLenThenLenRight) {
	CString str(NULL);
	EXPECT_EQ(str.getLen(), 0);
}

TEST_F(CStringTest, whenAppendCharThenRight) {
	CString str;
	str.appendChar('h');
	EXPECT_STREQ(str.getBuf(), "h");
}

TEST_F(CStringTest, whenAppendCharMakeStringLenOverMinLenThenRight) {
	CString str;
	for(int i = 0 ; i < 16; i ++)
		str.appendChar('h');
	EXPECT_EQ(str.getLen(), 16);
}

TEST_F(CStringTest, whenAppendStringThenRight) {
	CString str("hello");
	str.appendString("world");
	str.appendString("myos");
	EXPECT_STREQ(str.getBuf(), "helloworldmyos");
}

TEST_F(CStringTest, whenAppendStringMakeStringLenOverMinLenThenRight) {
	CString str;
	for (int i = 0; i < 16; i++)
		str.appendChar('h');
	str.appendChar('h');
	CString str1;
	str1.appendString(str.getBuf());
	EXPECT_EQ(str1.getLen(), 17);
}

TEST_F(CStringTest, strWhenAssignThenStrEq) {
	char testStr[] = "test";
	CString str = testStr;

	EXPECT_STREQ(str.getBuf(), testStr);
}

TEST_F(CStringTest, cstrWhenAssignThenStrEq) {
	CString str1("test");
	CString str = str1;

	EXPECT_STREQ(str.getBuf(), str1.getBuf());
}

TEST_F(CStringTest, cstrChangedWhenAssignThenStrEq) {
	CString str1("test");
	CString str = str1;

	str1 = "aaa";

	EXPECT_STRNE(str.getBuf(), str1.getBuf());
}

TEST_F(CStringTest, cstrWhenAddThenResultRight) {
	CString str = "hello ";
	CString str1 = "world";
	CString result = str + str1;
	EXPECT_STREQ(result.getBuf(), "hello world");
}

TEST_F(CStringTest, cstrWhenAddThenOriginalStrNotChange) {
	CString str = "hello ";
	CString str1 = "world";
	CString result = str + str1;
	EXPECT_STREQ(str.getBuf(), "hello ");
}

TEST_F(CStringTest, strWhenAddThenResultRight) {
	CString str = "hello ";
	CString result = str + "world";
	EXPECT_STREQ(result.getBuf(), "hello world");
}

TEST_F(CStringTest, allConstStringWhenFormatThenRight) {
	CString str;
	str.format("test");
	EXPECT_STREQ(str.getBuf(), "test");
}

TEST_F(CStringTest, sOfConstStringWhenFormatThenRight) {
	CString str;
	str.format("%s", "test");
	EXPECT_STREQ(str.getBuf(), "test");
}

TEST_F(CStringTest, dOfZeroIntegerWhenFormatThenRight) {
	CString str;
	str.format("%d", 0);
	EXPECT_STREQ(str.getBuf(), "0");
}

TEST_F(CStringTest, dOfOneIntegerWhenFormatThenRight) {
	CString str;
	str.format("%d", 1);
	EXPECT_STREQ(str.getBuf(), "1");
}

TEST_F(CStringTest, dOfHundredIntegerWhenFormatThenRight) {
	CString str;
	str.format("%d", 100);
	EXPECT_STREQ(str.getBuf(), "100");
}

TEST_F(CStringTest, dOfRandomIntegerWhenFormatThenRight) {
	CString str;
	str.format("%d", 1234);
	EXPECT_STREQ(str.getBuf(), "1234");
}

TEST_F(CStringTest, dOfMinuxIntegerWhenFormatThenRight) {
	CString str;
	str.format("%d", -1234);
	EXPECT_STREQ(str.getBuf(), "-1234");
}

TEST_F(CStringTest, xOfHexWhenFormatThenRight) {
	CString str;
	str.format("the number is :%x hello", 0xabc574);
	EXPECT_STREQ(str.getBuf(), "the number is :abc574 hello");
}

TEST_F(CStringTest, tOfCStringWhenFormatThenRight) {
	CString str;
	CString str1("hello world");
	str.format("%t", &str1);
	EXPECT_STREQ(str.getBuf(), "hello world");
}