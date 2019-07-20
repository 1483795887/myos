#include "pch.h"
#include <mm/physicalPage.h>
#include <mm/physicalPageAllocator/physicalPageAllocator.h>
#include <types.h>

class FakePhysicalPageAllocator :public PhysicalPageAllocator {
public:

};

class PhysicalPageManagerTest :public testing::Test {
public:
	virtual void SetUp() {
		testAddr = 0x401000;
		physicalPage = 0x123000;
		pt = NULL;
		pd = (PD)malloc(PAGE_SIZE);
		ppm = new PhysicalPageManager(pd);
		/*pd = (PD)malloc(PAGE_SIZE);
		pt = (PT)malloc(PAGE_SIZE);

		physicalPage = 0x1234 << 3;
		pd[1] = physicalPage;
		pd[2] = (PDE)pt;
		

		testAddr = 0x401000;
		testPTEAddr = 0x801000;

		pt[1] = physicalPage;*/
	}
	virtual void TearDown() {
		if(pt != NULL)
			free(pt);
		if(pd != NULL)
			free(pd);
	}
	PD pd;
	PT pt;
	ULONG testAddr;
	ULONG physicalPage;
	PhysicalPageManager* ppm;
};

TEST_F(PhysicalPageManagerTest, sizeTooBigWhenMapPagesThenFailed) {
	EXPECT_EQ(ppm->mapPages(0, 3 * G, G + 1, 0), SizeTooBig);
}

TEST_F(PhysicalPageManagerTest, testGetPDE) {
	pd[testAddr >> 22] = physicalPage;
	EXPECT_EQ(ppm->getPDE(testAddr), physicalPage);
}

TEST_F(PhysicalPageManagerTest, testGetPDEWhenPdIsTooHigh) {
	ppm->setPD((PD)(4 * G - K + 1));
	EXPECT_EQ(ppm->getPDE(testAddr), FAILED);
}

TEST_F(PhysicalPageManagerTest, testGetPTE) {
	pt = (PT)malloc(PAGE_SIZE);

	pd[testAddr >> 22] = (PDE)pt;

	pt[((testAddr >> 12) & 1023)] = physicalPage;
	EXPECT_EQ(ppm->getPTE(testAddr), physicalPage);
}

