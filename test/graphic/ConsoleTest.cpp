#include "pch.h"
#include <graphic/Graphic.h>
#include <graphic/Console.h>
#include <global/OS.h>
#include "FakeFont.h"

#define MAX_COL 16
#define MAX_ROW 16

class ConsoleTest : public testing::Test {
public:
    virtual void SetUp() {
        buffer = new BYTE[MAX_COL * MAX_ROW * 4 * 3];

		Rect graphicRect(0, 0, MAX_COL * 2, MAX_ROW * 2);

        globalGraphic = new Graphic;
        globalGraphic->init(&graphicRect, buffer);
        os->graphic = globalGraphic;

        rect = new Rect(0, 0, MAX_COL, MAX_ROW);
        console = new Console(rect);

        font = new FakeFont(rect->width, rect->height);
		fontColor = { 1,1,1 };
		font->setColor(fontColor);
        console->setFont(font);
    }
    virtual void TearDown() {
        delete console;
        delete font;
        delete rect;
        delete buffer;
        delete globalGraphic;
    }

	bool rgbCompare(RGB a, RGB b) {
		return a.r == b.r && a.g == b.g && a.b == b.b;
	}

    Rect* rect;
    Console* console;
    FakeFont* font;
    Graphic* globalGraphic;
    PBYTE buffer;

	RGB fontColor;
};

TEST_F(ConsoleTest, oneCharWhenPrintThenDataRight) {
    console->setCol(1);
    console->setRow(2);
    console->print("A");
    EXPECT_STREQ(font->getString(1, 2), "A");
}

TEST_F(ConsoleTest, oneCharOutOfColsWhenPrintThenNotChangePosition) {
    console->setCol(MAX_COL);
    console->print("A");
    EXPECT_STREQ(font->getString(MAX_COL, 0), "");
}

TEST_F(ConsoleTest, oneCharWhenPrintThenColInc) {
    console->setCol(2);
    ULONG col = console->getCol();
    console->print("A");
    EXPECT_EQ(col + 1, console->getCol());
}

TEST_F(ConsoleTest, oneCharInLastColWhenPrintThenColZero) {
    console->setCol(MAX_COL - 1);
    console->print("A");
    EXPECT_EQ(0, console->getCol());
}

TEST_F(ConsoleTest, oneCharNotInLastColWhenPrintThenRowNotChange) {
    console->setCol(2);
    ULONG row = console->getRow();
    console->print("A");
    EXPECT_EQ(row, console->getRow());
}

TEST_F(ConsoleTest, multiplyCharsNotAcrossMaxColsWhenPrintThenColAdd) {
    console->setCol(0);
    ULONG col = console->getCol();
    console->print("ABC");
    EXPECT_EQ(col + 3, console->getCol());
}

TEST_F(ConsoleTest, multiplyCharsAcrossMaxColsWhenPrintThenColRight) {
    console->setRow(0);
    ULONG row = console->getRow();
    console->setCol(MAX_COL - 2);
    console->print("ABC");
    EXPECT_EQ(1, console->getCol());
}

TEST_F(ConsoleTest, multiplyCharsAcrossMaxColsWhenPrintThenRowAdd) {
    console->setRow(1);
    ULONG row = console->getRow();
    console->setCol(MAX_COL - 2);
    console->print("ABC");
    EXPECT_EQ(row + 1, console->getRow());
}

TEST_F(ConsoleTest, multiplyCharsAcrossMaxColsWhenPrintThenDataRight) {
    console->setRow(1);
    ULONG row = console->getRow();
    console->setCol(MAX_COL - 2);
    console->print("ABC");
    EXPECT_STREQ("ABC", font->getString(MAX_COL - 2, 1));
}

TEST_F(ConsoleTest, rectNotFromZeroWhenPrintThenVamDataRight) {
	Rect newRect(1, 2, MAX_COL, MAX_ROW);
	console->setCol(1);
	console->setRow(2);
	console->setRect(&newRect);
	console->print("a");
	EXPECT_TRUE(rgbCompare(fontColor, os->graphic->getPixel(1 + 1, 2 + 2)));
}