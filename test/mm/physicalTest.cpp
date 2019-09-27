#include "pch.h"
#include <mm/PageMapper.h>
#include <mm/PageAllocator.h>
#include <types.h>
#include "FakePageAllocator.h"

class PageMapperManagerTest : public testing::Test {
public:
    virtual void SetUp() {
        testAddr = 0x401000;
        physicalPage = 0x123000;
        pt = NULL;

        allocator = new FakePageAllocator();
		pd = (PD)allocator->allocPages(0);
    }
    virtual void TearDown() {
        delete allocator;
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
    PageAllocator* allocator;
};

TEST_F(PageMapperManagerTest, sizeTooBigWhenMapPagesThenFailed) {
    EXPECT_EQ(PageMapper::mapPages(pd, 0, 3 * G, G + 1, 0, allocator), StatusMemoryOverLimit);
}

TEST_F(PageMapperManagerTest, pageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE, 0, allocator),
		StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, nextPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + PAGE_SIZE, pt, Existence);
    EXPECT_EQ(PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE * 2, 0, allocator),
		StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, lastPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + 9 * PAGE_SIZE, pt, Existence);
    EXPECT_EQ(PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE * 10, 0, allocator),
		StatusPageAlreadyExist);
}

TEST_F(PageMapperManagerTest, mapOnePageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
	PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE, Existence, allocator);
    EXPECT_EQ(PageMapper::va2pa(pd, testAddr), physicalPage);
}

TEST_F(PageMapperManagerTest, mapMultiplyPageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
	PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE * 10, Existence, allocator);
    EXPECT_EQ(PageMapper::va2pa(pd, testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapPagesThenTheAllocPage) {
	PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE, Existence, allocator);
    PT pt = (PT)((FakePageAllocator*)allocator)->getLastPage();

    EXPECT_EQ(pt[((testAddr >> 12) & 1023)] & ~0xfff, physicalPage);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapPagesThenMapSucceed) {
	PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE, Existence, allocator);

    EXPECT_EQ(PageMapper::va2pa(pd, testAddr), physicalPage);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenMapMultiplyPagesThenMappSucceed) {
	PageMapper::mapPages(pd, physicalPage, testAddr, PAGE_SIZE * 10, Existence, allocator);

    EXPECT_EQ(PageMapper::va2pa(pd, testAddr + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, acrossPDWhenMapMultiplyPagesThenMappSucceed) {
	PageMapper::mapPages(pd, physicalPage, 0x3ff000, PAGE_SIZE * 10, Existence, allocator);

    EXPECT_EQ(PageMapper::va2pa(pd, 0x3ff000 + PAGE_SIZE), physicalPage + PAGE_SIZE);
}

TEST_F(PageMapperManagerTest, pdNotExistWhenVA2PAThenFailed) {
    EXPECT_EQ(PageMapper::va2pa(pd, testAddr), NULL);
}

TEST_F(PageMapperManagerTest, ptNotExistWhenVA2PAThenFailed) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    EXPECT_EQ(PageMapper::va2pa(pd, testAddr), NULL);
}

TEST_F(PageMapperManagerTest, vA2PASuccess) {
    pt = (PT)allocator->allocPages(0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(PageMapper::va2pa(pd, testAddr), physicalPage);
}