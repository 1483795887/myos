#include "pch.h"
#include <mm/physicalPage.h>
#include <mm/physicalPageAllocator/physicalPageAllocator.h>
#include <types.h>

#define MAX_BLOCKS 10
class FakePhysicalPageAllocator : public PhysicalPageAllocator {
public:
    ULONG allocPages(Zone* zone, ULONG order) {
        ULONG number = 1 << order;
        ULONG size = number * PAGE_SIZE;
        if (currentBlock >= MAX_BLOCKS)
            return FAILED;
        ULONG buffer = (ULONG)_aligned_malloc(size, PAGE_SIZE);
        memset((void*)buffer, 0, size);
        blocks[currentBlock] = buffer;
        currentBlock++;
        return buffer;
    }

    FakePhysicalPageAllocator() {
        currentBlock = 0;
        for (int i = 0; i < MAX_BLOCKS; i++)
            blocks[i] = NULL;
    }

    ~FakePhysicalPageAllocator() {
        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (blocks[i] != NULL) {
                free((void*)blocks[i]);
                blocks[i] = NULL;
            }

        }
    }
private:
    ULONG blocks[MAX_BLOCKS];
    ULONG currentBlock;
};

class PhysicalPageManagerTest : public testing::Test {
public:
    virtual void SetUp() {
        testAddr = 0x401000;
        physicalPage = 0x123000;
        pt = NULL;

        allocator = new FakePhysicalPageAllocator();
        pd = (PD)allocator->allocPages(NULL, 0);
        ppm = new PhysicalPageManager(pd);
        int i = 0;
    }
    virtual void TearDown() {
        delete allocator;
    }

    void mapPDE(ULONG vAddr, PT pt, PD pd, ULONG property) {
        if (pd  != NULL)
            pd[vAddr >> 22] = (PDE)((ULONG)pt | (property & 0xfff));
    }

    void mapPTE(ULONG pAddr, ULONG physcialPage, PT pt, ULONG property) {
        if (pt != NULL)
            pt[((pAddr >> 12) & 1023)] = physicalPage | (property & 0xfff);
    }

    PD pd;
    PT pt;
    ULONG testAddr;
    ULONG physicalPage;
    PhysicalPageManager* ppm;
    PhysicalPageAllocator* allocator;
};

TEST_F(PhysicalPageManagerTest, sizeTooBigWhenMapPagesThenFailed) {
    EXPECT_EQ(ppm->mapPages(0, 3 * G, G + 1, 0), SizeTooBig);
}

TEST_F(PhysicalPageManagerTest, pageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(NULL, 0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, nextPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(NULL, 0);
	mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + PAGE_SIZE, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 2, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, lastPageAlreadyExistWhenMapPagesThenFailed) {
    pt = (PT)allocator->allocPages(NULL, 0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage + 9 * PAGE_SIZE, pt, Existence);
    EXPECT_EQ(ppm->mapPages(physicalPage, testAddr, PAGE_SIZE * 10, 0), PageAlreadyExist);
}

TEST_F(PhysicalPageManagerTest, mapOnePageWhenMapPagesThenSuccess) {
    pt = (PT)allocator->allocPages(NULL, 0);
    mapPDE(testAddr, pt, pd, Existence);
    ppm->mapPages(physicalPage, testAddr, PAGE_SIZE, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), physicalPage);
}

TEST_F(PhysicalPageManagerTest, pdNotExistWhenVA2PAThenFailed) {
    EXPECT_EQ(ppm->va2pa(testAddr), PDNotExist);
}

TEST_F(PhysicalPageManagerTest, ptNotExistWhenVA2PAThenFailed) {
    pt = (PT)allocator->allocPages(NULL, 0);
    mapPDE(testAddr, pt, pd, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), PTNotExist);
}

TEST_F(PhysicalPageManagerTest, vA2PASuccess) {
    pt = (PT)allocator->allocPages(NULL, 0);
    mapPDE(testAddr, pt, pd, Existence);
    mapPTE(testAddr, physicalPage, pt, Existence);
    EXPECT_EQ(ppm->va2pa(testAddr), physicalPage);
}