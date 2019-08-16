#include "pch.h"
#include <global/OS.h>
#include <graphic/Graphic.h>

class GraphicTest :public testing::Test {
public:
	virtual void SetUp() {
		vam = (PBYTE)malloc(4096);
		os->setLastStatus(Success);

		FILE* hFile = fopen("boot.fnt", "r");
		fseek(hFile, 0, SEEK_END);
		int size = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		charMap = (PBYTE)malloc(size);
		fread(charMap, 1, size, hFile);
		fclose(hFile);
	}
	virtual void TearDown() {
		free(vam);
		free(charMap);
	}

	bool rgbCompare(RGB a, RGB b) {
		return a.r == b.r && a.g == b.g && a.b == b.b;
	}

	Graphic video;
	PBYTE vam;
	PBYTE charMap;
};

TEST_F(GraphicTest, vamNullWhenInitThenReturnStatus) {
	Rect rect(0, 0, 10, 10);
	EXPECT_EQ(video.init(&rect, NULL), NullPointer);
}

TEST_F(GraphicTest, xZeroWhenInitThenReturnStatus) {
	Rect rect(0, 0, 0, 10);
	EXPECT_EQ(video.init(&rect, vam), ValueNotInRange);
}

TEST_F(GraphicTest, yZeroWhenInitThenReturnStatus) {
	Rect rect(0, 0, 10, 0);
	EXPECT_EQ(video.init(&rect, vam), ValueNotInRange);
}

TEST_F(GraphicTest, xHitLimitWhenGetPixelThenReturnDefault) {
	Rect rect(0, 0, 10, 10);
	video.init(&rect, vam);
	RGB rgb = video.getPixel(10, 0);
	EXPECT_TRUE(rgbCompare(rgb, DEFAULT_COLOR));
}

TEST_F(GraphicTest, xOverLimitWhenGetPixelThenReturnDefault) {
	Rect rect(0, 0, 10, 10);
	video.init(&rect, vam);
	RGB rgb = video.getPixel(11, 0);
	EXPECT_TRUE(rgbCompare(rgb, DEFAULT_COLOR));
}

TEST_F(GraphicTest, yHitLimitWhenPutPixelThenPixelNotChange) {
	Rect rect(0, 0, 10, 10);
	video.init(&rect, vam);
	RGB color = { 1,2,3, };
	EXPECT_EQ(video.putPixel(0, 10, color), ArrayBounds);
}

TEST_F(GraphicTest, yOverLimitWhenPutPixelThenPixelNotChange) {
	Rect rect(0, 0, 10, 10);
	video.init(&rect, vam);
	RGB color = { 1,2,3, };
	EXPECT_EQ(video.putPixel(0, 11, color), ArrayBounds);
}