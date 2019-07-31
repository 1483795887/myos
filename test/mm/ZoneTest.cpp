#include "pch.h"
#include <mm/zone.h>
#include "FakePhysicalPageAllocator.h"
#include <mm/Page.h>

class FreeAreaTest :public testing::Test {
public:
	virtual void SetUp() {

	}

	virtual void TearDown() {

	}
};

TEST_F(FreeAreaTest, addressNotAlignedWhenLinkFreePagesThenReturnFaild) {
	FreeArea freeArea;
	EXPECT_EQ(freeArea.LinkFreePages((PBYTE)(4 * K - 1), 4 * K), AddressNotAligned);
}

TEST_F(FreeAreaTest, sizeNotAlignedWhenLinkFreePagesThenReturnFaild) {
	FreeArea freeArea;
	EXPECT_EQ(freeArea.LinkFreePages((PBYTE)(4 * K), (4 * K - 1)), AddressNotAligned);
}

TEST_F(FreeAreaTest, sizeTooBigWhenLinkFreePagesThenReturnFaild) {
	FreeArea freeArea;
	EXPECT_EQ(freeArea.LinkFreePages((PBYTE)(4 * G - 4 * K), (8 * K)), MemoryOverLimit);
}

TEST_F(FreeAreaTest, linkOnePageWhenLinkFreePagesThenCountOne) {
	FreeArea freeArea;
	FakePhysicalPageAllocator allocator;
	PBYTE buffer =  allocator.allocPages(NULL, 0);
	freeArea.LinkFreePages(buffer, PAGE_SIZE);
	EXPECT_EQ(freeArea.getList()->getCount(), 1);
}

TEST_F(FreeAreaTest, linkOnePageWhenLinkFreePagesThenAddressRight) {
	FreeArea freeArea;
	FakePhysicalPageAllocator allocator;
	PBYTE buffer = allocator.allocPages(NULL, 0);
	freeArea.LinkFreePages(buffer, PAGE_SIZE);
	Page* page = (Page*)freeArea.getList()->getFirst();
	EXPECT_EQ(page->getData(), buffer);
}

TEST_F(FreeAreaTest, linkTwoPageWhenLinkFreePagesThenCountTwo) {
	FreeArea freeArea;
	FakePhysicalPageAllocator allocator;
	PBYTE buffer = allocator.allocPages(NULL, 1);
	freeArea.LinkFreePages(buffer, PAGE_SIZE * 2);
	EXPECT_EQ(2, freeArea.getList()->getCount());
}

TEST_F(FreeAreaTest, linkTwoPageWhenLinkFreePagesThenCheckLoop) {
	FreeArea freeArea;
	FakePhysicalPageAllocator allocator;
	PBYTE buffer = allocator.allocPages(NULL, 1);
	freeArea.LinkFreePages(buffer, PAGE_SIZE * 2);
	Page* page = (Page*)freeArea.getList()->getFirst();
	Page* nextPage = (Page*)page->getNext();
	nextPage = (Page*)nextPage->getNext();
	EXPECT_EQ(nextPage, freeArea.getList()->getHead());
}