#include "pch.h"
#include <mm/zone.h>
#include "FakePhysicalPageAllocator.h"
#include <mm/Page.h>
#include <mm/FakePool.h>
#include <mm/NaivePool.h>
#include <mm/FakePhysicalPageAllocator.h>
#include <global/OS.h>


class ZoneTest : public testing::Test {
public:
    virtual void SetUp() {
        fakePool = new FakePool(1024);
        os->pool = fakePool;

        allocator = new FakePhysicalPageAllocator();
    }

    virtual void TearDown() {
        delete fakePool;
        delete allocator;
    }

    FakePool* fakePool;
    PhysicalPageAllocator* allocator;
};

TEST_F(ZoneTest, orderNotInRangeWhenInitFreeAreaThenError) {
    FreeArea freeArea;
    EXPECT_EQ(freeArea.init(0, 1111, 10), ValueNotInRange);
}

TEST_F(ZoneTest, emptyZoneWhenGetFreePagesThenZero) {
    Zone zone;
    zone.init(allocator->allocPages(NULL, 0), 0);
    EXPECT_EQ(zone.getFreePages(), 0);
}

TEST_F(ZoneTest, pageNotInZoneWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 1);
    zone.init(address + PAGE_SIZE, PAGE_SIZE);

    Page* page = (Page*)address;
    page->setOrder(0);

    zone.putPages(page);

    EXPECT_EQ(zone.putPages(page), InvalidPage);
}

TEST_F(ZoneTest, onePageAddressNotAlignedWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE);

    Page* page = (Page*)address + 16;
    page->setOrder(0);

    EXPECT_EQ(zone.putPages(page), InvalidPage);
}

TEST_F(ZoneTest, onePageInvalidWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE);

    Page page;
    page.setOrder(0);

    EXPECT_EQ(zone.putPages(&page), InvalidPage);
}


TEST_F(ZoneTest, onePageOrderNotInRangeWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE);

    Page* page = (Page*)address;
    page->setOrder(11);

    EXPECT_EQ(zone.putPages(page), ValueNotInRange);
}


TEST_F(ZoneTest, onePageWhenPutPagesThenRelatedAreaCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE);

    Page* page = (Page*)address;
    page->setOrder(0);

    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 1);
}

TEST_F(ZoneTest, twoNeighborOnePagesFirstLowThenHighWhenPutPagesThenUpperAreaCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 1);
    zone.init(address, PAGE_SIZE * 2);

    Page* page = (Page*)address;
    page->setOrder(0);

    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE);
    page->setOrder(0);
    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(1), 1);
}

TEST_F(ZoneTest, twoNeighborOnePagesFirstHighThenLowWhenPutPagesThenCurrentAreaCountZero) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 1);
    zone.init(address, PAGE_SIZE * 2);

    Page* page = (Page*)(address + PAGE_SIZE);
    page->setOrder(0);

    zone.putPages(page);

    page = (Page*)address;
    page->setOrder(0);
    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 0);
}

TEST_F(ZoneTest, twoNotNeighborOnePagesWhenPutPagesThenAreaOneCountTwo) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 2);
    zone.init(address, PAGE_SIZE * 4);

    Page* page = (Page*)address;
    page->setOrder(0);
    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE * 2);
    page->setOrder(0);
    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 2);
}


TEST_F(ZoneTest, fourNeighborOnePagesWhenPutPagesThenAreaTwoCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 2);
    zone.init(address, PAGE_SIZE * 4);

    Page* page = (Page*)address;
    page->setOrder(0);
    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE);
    page->setOrder(0);
    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE * 2);
    page->setOrder(0);
    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE * 3);
    page->setOrder(0);
    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(2), 1);
}


TEST_F(ZoneTest, twoOrderMaxNeighborPagesWhenPutPagesThenAreaTwoNoMerge) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 11);
    zone.init(address, PAGE_SIZE  * K * 2);

    Page* page = (Page*)address;
    page->setOrder(10);
    zone.putPages(page);

    page = (Page*)(address + PAGE_SIZE * K);
    page->setOrder(10);
    zone.putPages(page);

    EXPECT_EQ(zone.getFreePagesForOrder(10), 2);
}

