#include <pch.h>
#include <global/OS.h>
using namespace testing;

int main(int argc, char** argv) {
    os = new OS();

    InitGoogleTest(&argc, argv);
    FLAGS_gtest_filter = "ConsoleTest.*";

    int result = RUN_ALL_TESTS();

    delete os;

    return result;
}