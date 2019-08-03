#include <pch.h>
#include <global/OS.h>
using namespace testing;


OS* os = NULL;

int main(int argc, char** argv) {
	os = new OS();

	InitGoogleTest(&argc, argv);
	//FLAGS_gtest_filter = "NaivePoolTest.*";

	int result = RUN_ALL_TESTS();

	delete os;

	return result;
}