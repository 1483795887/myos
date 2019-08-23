#include "pch.h"
#include <mm/physicalPage.h>
#include <mm/physicalPageAllocator.h>
#include <types.h>
#include "FakePhysicalPageAllocator.h"

class PhysicalPageManagerTest : public testing::Test {
public:
    virtual void SetUp() {
        testAddr = 0x401000;
        physicalPage = 0x123000;
        pt = NULL;

        allocator = new FakePhysicalPageAllocator();
        pd = (PD)allocator->allocPages(0);
        ppm = new PhysicalPageManager;
        ppm->setPD(pd);
        ppm->setAllocator(allocator);
    }
    virtual void TearDown() {
        delete allocator;
        delete ppm;
    }

    void mapPDE(ULONG vAddr, PT pt, PD pd, ULONG property) {
        if (pd  != NULL)
            pd[vAddr >> 22] = (PDE)((ULONG)pt | (property & 0xfff));
    }

    void mapPTE(ULONG vAddr, ULONG physcialPage, PT pt, ULONG property) {
        if (pt != NULL)
            pt[((vAddr >> 12) & 1023)] = physicalPage | (property & 0xfff);
    }

    PD pd;
    PT pt;
    ULONG testAddr;
    ULONG physicalPage;
    PhysicalPageManager* ppm;
    PhysicalPageAllocator* allocator;
};

TEST_F(PhysicalPageManagerTest, sizeTooBigWhenMapPagesThenFailed) {
    EXPECT_EQ(ppm->mapPages(0, 3 * G, G + 1, 0), MemoryOverLimit);
}

TEST_F(PhysicalPageManagerTest, pageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, nextPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + PAGE_SIZE, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 2, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, lastPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + 9 * PAGE_SIZE, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, mapOnePageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), physicalPage);
}

TEST_F(PhysicalPageManagerTest, mapMultiplyPageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PhysicalPageManagerTest, ptNotExistWhenMapPagesThenTheAllocPage) {
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);
    PT pt = (PT)((FakePhysicalPageAllocator*)allocator)->getLastPage();

    EXPECT_EQ(pt[((testAddr >> 12) & 1023)] & ~0xfff, physicalPage);
}

TEST_F(PhysicalPageManagerTest, ptNotExistWhenMapPagesThenMapSucceed) {
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);

    EXPECT_EQ(ppm->va2pa(testAddr), physicalPage);
}

TEST_F(PhysicalPageManagerTest, ptNotExistWhenMapMultiplyPagesThenMappSucceed) {
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, Existence);

    EXPECT_EQ(ppm->va2pa(testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PhysicalPageManagerTest, acrossPDWhenMapMultiplyPagesThenMappSucceed) {
    ppm->mapPages(physicalPage, 0x3ff000, PAGE_SIZE * 10, Existence);

    EXPECT_EQ(ppm->va2pa(0x3ff000 + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PhysicalPageManagerTest, pdNotExistWhenVA2PAThenFailed) {
    EXPECT_EQ(ppm->va2pa(testAddr), NULL);
}

TEST_F(PhysicalPageManagerTest, ptNotExistWhenVA2PAThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), NULL);
}

TEST_F(PhysicalPageManagerTest, vA2PASuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), physicalPage);
}