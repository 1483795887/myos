#include "pch.h"
#include <fs/pe.h>


TEST(PeTest, TestName) {
	PeFileParser parser(0);
	EXPECT_EQ(FALSE, parser.isFileValid());
}