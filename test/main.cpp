#include <pch.h>
#include <global/OS.h>
using namespace testing;

void operator delete(void* ptr) {
	if (os == NULL) {
		free(ptr);
	}
	else if (os->pool == NULL) {
		free(ptr);
	}
	else if (ptr == os) {
		free(ptr);
	}
	else if (ptr == os->pool) {
		free(ptr);
		os->pool = NULL;
	}
	else if (os->pool->isInPool((PBYTE)ptr)) {
		os->pool->free((PBYTE)ptr);
	}
	else {
		free(ptr);
	}
}

int main(int argc, char** argv) {
    os = new OS();
    InitGoogleTest(&argc, argv);
    ///FLAGS_gtest_filter = "VirtualMemoryManagerTest.*";

    int result = RUN_ALL_TESTS();

    delete os;
	os = NULL;

    return result;
}