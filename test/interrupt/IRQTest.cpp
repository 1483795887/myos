#include "pch.h"
#include <interrupt/IRQ.h>
#include <Status.h>

class TestInterrupter:public Interrupter {
public:
	virtual DWORD handle(TrapFrame* frame) {
		return StatusSuccess;
	}
};

class IRQTest : public testing::Test {
public:
    virtual void SetUp() {
		interrupter = new TestInterrupter;
    }

    virtual void TearDown() {

    }
	TrapFrame frame;
    IRQ irq;
	TestInterrupter* interrupter;
};

TEST_F(IRQTest, TrapFrameNullWhenHandleThenReturnNullPointer) {
	EXPECT_EQ(irq.handle(NULL), StatusNullPointer);
}

TEST_F(IRQTest, NoInterrupterWhenHandleThenReturnNullPointer) {
	EXPECT_EQ(irq.handle(&frame), StatusNullPointer);
}

TEST_F(IRQTest, NullWhenBindAndHandleThenReturnNullPointer) {
	irq.bindInterrupter(NULL);
	EXPECT_EQ(irq.handle(&frame), StatusNullPointer);
}

TEST_F(IRQTest, ValidWhenBindAndHandleThenReturnResultOfInterrupter) {
	irq.bindInterrupter(interrupter);
	EXPECT_EQ(irq.handle(&frame), StatusSuccess);
}