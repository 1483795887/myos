#include "pch.h"
#include <mm/PageMapper.h>
#include <mm/physicalPageAllocator.h>
#include <types.h>
#include "FakePhysicalPageAllocator.h"

class PageMapperManagerTest : public testing::Test {
public:
    virtual void SetUp() {
        testAddr = 0x401000;
        physicalPage = 0x123000;
        pt = NULL;

        allocator = new FakePhysicalPageAllocator();
        pd = (PD)allocator->allocPages(0, NOT_ASSIGNED);
        pageMapper = new PageMapper;
        pageMapper->setPD(pd);
        pageMapper->setAllocator(allocator);
    }
    virtual void TearDown() {
        delete allocator;
        delete pageMapper;
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
    PageMapper* pageMapper;
    PhysicalPageAllocator* allocator;
};

TEST_F(PageMapperManagerTest, sizeTooBigWhenMapPagesThenFailed) {
    EXPECT_EQ(pageMapper->mapPages(0, 3 * G, G + 1, 0), StatusMemoryOverLimit);
}

TEST_F(PageMapperManagerTest, pageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE, 0), StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, nextPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + PAGE_SIZE, pt, Existence);
    EXPECT_EQ(pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE * 2, 0), StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, lastPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + 9 * PAGE_SIZE, pt, Existence);
    EXPECT_EQ(pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, 0), StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, mapOnePageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);
    EXPECT_EQ(pageMapper->va2pa(testAddr), physicalPage);
}

TEST_F(PageMapperManagerTest, mapMultiplyPageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, Existence);
    EXPECT_EQ(pageMapper->va2pa(testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapPagesThenTheAllocPage) {
    pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);
    PT pt = (PT)((FakePhysicalPageAllocator*)allocator)->getLastPage();

    EXPECT_EQ(pt[((testAddr >> 12) & 1023)] & ~0xfff, physicalPage);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapPagesThenMapSucceed) {
    pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);

    EXPECT_EQ(pageMapper->va2pa(testAddr), physicalPage);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapMultiplyPagesThenMappSucceed) {
    pageMapper->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, Existence);

    EXPECT_EQ(pageMapper->va2pa(testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, acrossPDWhenMapMultiplyPagesThenMappSucceed) {
    pageMapper->mapPages(physicalPage, 0x3ff000, PAGE_SIZE * 10, Existence);

    EXPECT_EQ(pageMapper->va2pa(0x3ff000 + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, pdNotExistWhenVA2PAThenFailed) {
    EXPECT_EQ(pageMapper->va2pa(testAddr), NULL);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenVA2PAThenFailed) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    EXPECT_EQ(pageMapper->va2pa(testAddr), NULL);
}

TEST_F(PageMapperManagerTest, vA2PASuccess) {
    pt = (PT)allocator->allocPages(0, NOT_ASSIGNED);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(pageMapper->va2pa(testAddr), physicalPage);
}