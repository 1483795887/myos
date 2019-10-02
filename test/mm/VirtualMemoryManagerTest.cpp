#include "pch.h"
#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/VirtualMemoryManager.h>
#include "FakePool.h"

class VirtualMemoryManagerTest : public testing::Test {
public:
    virtual void SetUp() {

        fakePool = new FakePool(PAGE_SIZE * 32);
        os->pool = fakePool;

        vmm = New VirtualMemoryManager; //内部用到了new
    }
    virtual void TearDown() {
        delete vmm;
        delete fakePool;
    }

    VirtualMemoryManager* vmm;
    FakePool* fakePool;
};

TEST_F(VirtualMemoryManagerTest, sizeNotAlignedWhenAllocateThenReturnZero) {
    EXPECT_EQ(vmm->allocate(0x123, VMWritable), 0);
}

TEST_F(VirtualMemoryManagerTest, onePageWhenAllocateThenGetMinMemory) {
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), MIN_MEMORY);
}

TEST_F(VirtualMemoryManagerTest, twoPageWhenAllocateThenAddrIsAdjoined) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr + PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest, fourPagesAndTwoPagesWhenAllocateThenAddrIsAdjoined) {
    ULONG addr = vmm->allocate(PAGE_SIZE * 4, VMWritable);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE * 2, VMWritable), addr + PAGE_SIZE * 4);
}

TEST_F(VirtualMemoryManagerTest, sizeOntEnoughWhenAllocateThenReturnZero) {
    EXPECT_EQ(vmm->allocate(4 * G - 1, VMWritable), 0);
}

TEST_F(VirtualMemoryManagerTest, addrNotAlignedWhenReleaseThenNotRelease) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(0x123, PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr + PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest, sizeNotAlignedWhenReleaseThenNotRelease) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(addr, PAGE_SIZE - 1);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr + PAGE_SIZE);
}


TEST_F(VirtualMemoryManagerTest, oneOfOneWhenReleaseThenAllocateResultSame) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(addr, PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr);
}

TEST_F(VirtualMemoryManagerTest, laterOfTwoWhenReleaseThenAllocateResultSameWithLater) {
    ULONG addr = vmm->allocate(PAGE_SIZE * 2, VMWritable);
    vmm->release(addr + PAGE_SIZE, PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr + PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest, middleOfThreeWhenReleaseThenAllocateResultSameWithMiddle) {
    ULONG addr = vmm->allocate(PAGE_SIZE * 3, VMWritable);
    vmm->release(addr + PAGE_SIZE, PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE, VMWritable), addr + PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest,
       threeOnePageReleaseLastTwoWhenAllocateOneTwoPagesThenReturnSameWithSecondOne) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(addr + PAGE_SIZE, 2 * PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE * 2, VMWritable), addr + PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest,
       threeOnePageReleaseFirstTwoWhenAllocateOneTwoPagesThenReturnSameWithFirstOne) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(addr, 2 * PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE * 2, VMWritable), addr);
}

TEST_F(VirtualMemoryManagerTest,
       threeOnePageReleaseTheMiddleWhenAllocateOneTwoPagesThenReturnNewArea) {
    ULONG addr = vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->allocate(PAGE_SIZE, VMWritable);
    vmm->release(addr + PAGE_SIZE,  PAGE_SIZE);
    EXPECT_EQ(vmm->allocate(PAGE_SIZE * 2, VMWritable), addr + 3 * PAGE_SIZE);
}

TEST_F(VirtualMemoryManagerTest,
       threePagesReleaseOneByOneWhenAllocateThreePageThenReturnSameWithTheFirst) {
    ULONG addr = vmm->allocate(PAGE_SIZE * 3, VMWritable);

    vmm->release(addr,  PAGE_SIZE);
    vmm->release(addr + PAGE_SIZE * 2, PAGE_SIZE);

    vmm->release(addr + PAGE_SIZE, PAGE_SIZE);

    EXPECT_EQ(vmm->allocate(PAGE_SIZE * 3, VMWritable), addr);
}