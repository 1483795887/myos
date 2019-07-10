#include "pch.h"
#include <mm/physicalPage.h>
#include <types.h>


class PhysicalPageManagerTest :public testing::Test {
public:
	virtual void SetUp() {
		pd = (PD)malloc(PAGE_SIZE);
		pt = (PT)malloc(PAGE_SIZE);

		physicalPage = 0x1234 << 3;
		pd[1] = physicalPage;
		pd[2] = (PDE)pt;
		

		testAddr = 0x401000;
		testPTEAddr = 0x801000;

		pt[1] = physicalPage;
		ppm = new PhysicalPageManager(pd);
	}
	virtual void TearDown() {
		free(pd);
	}
	PD pd;
	PT pt;
	ULONG testAddr;
	ULONG testPTEAddr;
	ULONG physicalPage;
	PhysicalPageManager* ppm;
};

TEST_F(PhysicalPageManagerTest, testGetPDE) {
	EXPECT_EQ(ppm->getPDE(testAddr), physicalPage);
}

TEST_F(PhysicalPageManagerTest, testGetPDEWhenPdIsTooHigh) {
	ppm->setPD((PD)(4 * G - K + 1));
	EXPECT_EQ(ppm->getPDE(testAddr), FAILED);
}

TEST_F(PhysicalPageManagerTest, testGetPTE) {
	EXPECT_EQ(ppm->getPTE(testPTEAddr), physicalPage);
}