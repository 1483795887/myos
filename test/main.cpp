#include <pch.h>
using namespace testing;

int main(int argc, char** argv) {
	InitGoogleTest(&argc, argv);
	FLAGS_gtest_filter = "FreeAreaTest.*";
	return RUN_ALL_TESTS();
}