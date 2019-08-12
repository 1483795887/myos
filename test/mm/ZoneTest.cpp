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
        fakePool = new FakePool(PAGE_SIZE * 32);
        os->pool = fakePool;
        os->setLastStatus(Success);
        allocator = new FakePhysicalPageAllocator();
        zone = new Zone();

		pages = new Page[4096];
    }

    virtual void TearDown() {
        delete fakePool;
        delete allocator;
        delete zone;
		delete[] pages;
    }

    PBYTE initZone(ULONG order) {
        PBYTE address = allocator->allocPages(NULL, order);
        zone->init(address, getPageSizeByOrder(order), pages);
        zone->putAllPages();
        return address;
    }

    FakePool* fakePool;
    PhysicalPageAllocator* allocator;
    Zone* zone;
	Page* pages;
};

TEST_F(ZoneTest, orderNotInRangeWhenInitFreeAreaThenError) {
    FreeArea freeArea;
    EXPECT_EQ(freeArea.init(0, 1111, 10), ValueNotInRange);
}

TEST_F(ZoneTest, emptyZoneWhenGetFreePagesThenZero) {
    Zone zone;
    zone.init(allocator->allocPages(NULL, 0), 0, pages);
    EXPECT_EQ(zone.getFreePages(), 0);
}

TEST_F(ZoneTest, pageNotInZoneWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE, pages);

    EXPECT_EQ(zone.putPage(0), InvalidPage);
}

TEST_F(ZoneTest, onePageAddressNotAlignedWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE, pages);

    EXPECT_EQ(zone.putPage((PBYTE)16), InvalidPage);
}

TEST_F(ZoneTest, onePageOrderLowerThanRangeWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE, pages);

    EXPECT_EQ(zone.putPage((PBYTE)0), InvalidPage);
}

TEST_F(ZoneTest, onePageOrderOverThanRangeWhenPutPagesThenReturnStatus) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE, pages);

    EXPECT_EQ(zone.putPage(address + PAGE_SIZE), InvalidPage);
}

TEST_F(ZoneTest, onePageWhenPutPagesThenRelatedAreaCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 0);
    zone.init(address, PAGE_SIZE, pages);

    zone.putPage(address);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 1);
}

TEST_F(ZoneTest, twoNeighborOnePagesFirstLowThenHighWhenPutPagesThenUpperAreaCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 1);
    zone.init(address, PAGE_SIZE * 2, pages);

    zone.putPage(address);
    zone.putPage(address + PAGE_SIZE);

    EXPECT_EQ(zone.getFreePagesForOrder(1), 1);
}

TEST_F(ZoneTest, twoNeighborOnePagesFirstHighThenLowWhenPutPagesThenCurrentAreaCountZero) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 1);
    zone.init(address, PAGE_SIZE * 2, pages);

    zone.putPage(address + PAGE_SIZE);
    zone.putPage(address);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 0);
}

TEST_F(ZoneTest, twoNotNeighborOnePagesWhenPutPagesThenAreaOneCountTwo) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 2);
    zone.init(address, PAGE_SIZE * 4, pages);

    zone.putPage(address);
    zone.putPage(address + PAGE_SIZE * 2);

    EXPECT_EQ(zone.getFreePagesForOrder(0), 2);
}

TEST_F(ZoneTest, fourNeighborOnePagesWhenPutPagesThenAreaTwoCountOne) {
    Zone zone;
    PBYTE address = allocator->allocPages(NULL, 2);
    zone.init(address, PAGE_SIZE * 4, pages);

    zone.putPage(address);
    zone.putPage(address + PAGE_SIZE);
    zone.putPage(address + PAGE_SIZE * 2);
    zone.putPage(address + PAGE_SIZE * 3);

    EXPECT_EQ(zone.getFreePagesForOrder(2), 1);
}

TEST_F(ZoneTest, twoOrderMaxPagesWhenPutAllPagesThenAreaTenCountTwo) {
    initZone(11);
    EXPECT_EQ(zone->getFreePagesForOrder(10), 2);
}

TEST_F(ZoneTest, orderNotInRangeWhenGetPagesThenReturnZero) {
    initZone(0);
    EXPECT_EQ(zone->getPages(123), (PBYTE)NULL);
}

TEST_F(ZoneTest, onePageWhenGetPagesThenAddressSame) {
    PBYTE address = initZone(0);
    EXPECT_EQ(zone->getPages(0), address);
}

TEST_F(ZoneTest, onePageFromNoneWhenGetPagesThenReturnZero) {
	initZone(0);
	zone->getPages(0);
	EXPECT_EQ(zone->getPages(0), (PBYTE)NULL);
}

TEST_F(ZoneTest, onePageFromTwoWhenGetPagesThenAreaTwoCountZero) {
	initZone(1);
	zone->getPages(0);
	EXPECT_EQ(zone->getFreePagesForOrder(1), 0);
}

TEST_F(ZoneTest, onePageFromTwoWhenGetPagesThenAreaOneCountOne) {
	initZone(1);
	zone->getPages(0);
	EXPECT_EQ(zone->getFreePagesForOrder(0), 1);
}

TEST_F(ZoneTest, getMaxOrderWhenNotEnoughWhenGetPagesThenReturnNull) {
	initZone(9);
	EXPECT_EQ(zone->getPages(10), (PBYTE)NULL);
}

TEST_F(ZoneTest, oneOrderTwoPageFromOneOrderOnePageWhenGetPagesThenReturnNull) {
	initZone(1);
	EXPECT_EQ(zone->getPages(2), (PBYTE)NULL);
}