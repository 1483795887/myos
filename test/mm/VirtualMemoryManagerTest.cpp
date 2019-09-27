#include "pch.h"
#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/VirtualMemoryManager.h>
#include "FakePool.h"

class VirtualMemoryManagerTest : public testing::Test {
public:
    virtual void SetUp() {
        vmm = new VirtualMemoryManager;
		fakePool = new FakePool(PAGE_SIZE * 32);
		os->pool = fakePool;
    }
    virtual void TearDown() {
		delete fakePool;
        delete vmm;
    }

    VirtualMemoryManager* vmm;
	FakePool* fakePool;
};

TEST_F(VirtualMemoryManagerTest, emptyAreaListWhenGetInfoThenReturnNull) {
    VirtualMemoryInfo* info = vmm->getVirtualMemoryInfo(0x12345655);
    EXPECT_EQ(info, (VirtualMemoryInfo*)NULL);
}

TEST_F(VirtualMemoryManagerTest, addrNotAlignedWhenInsertThenReturnStatus) {
    VirtualMemoryInfo info(0x1234, PAGE_SIZE, VMWritable);
    EXPECT_EQ(vmm->addMemoryArea(info), StatusNotAligned);
}

TEST_F(VirtualMemoryManagerTest, sizeNotAlignedWhenInsertThenReturnStatus) {
    VirtualMemoryInfo info(PAGE_SIZE * 10, PAGE_SIZE * 2 - 1, VMWritable);
    EXPECT_EQ(vmm->addMemoryArea(info), StatusNotAligned);
}

TEST_F(VirtualMemoryManagerTest, getInfoAfterInsertedThenPropertyRight) {
    ULONG addr = 0x20000;
    VirtualMemoryInfo info(addr, PAGE_SIZE * 2 , VMWritable);
    vmm->addMemoryArea(info);
    VirtualMemoryInfo* nInfo = vmm->getVirtualMemoryInfo(addr);
    EXPECT_EQ(nInfo->property, VMWritable);
}