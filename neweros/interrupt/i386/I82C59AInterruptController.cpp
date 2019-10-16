#include <interrupt/InterruptController.h>
#include "..\..\include\interrupt\i386\I82C59AInterruptController.h"
#include <arch/IO.h>


I82C59AInterruptController::~I82C59AInterruptController() {
}

void I82C59AInterruptController::EnableInterrupt(IntType type) {
}

void I82C59AInterruptController::DisableInterrupt(IntType type) {
}

void I82C59AInterruptController::init() {
	Out8(PortIntMasterControl, (ICW1_BASE & ~SINGLE_CHIP) | WRITE_ICW4);
	Out8(PortIntMasterControlMask, MASTER_INT_VEC);
	Out8(PortIntMasterControlMask, MASTER_ICW3);
	Out8(PortIntMasterControlMask, ICW4_BASE);

	Out8(PortIntServantControl, (ICW1_BASE & ~SINGLE_CHIP) | WRITE_ICW4);
	Out8(PortIntServantControlMask, SERVANT_INT_VEC);
	Out8(PortIntServantControlMask, SERVANT_ICW3);
	Out8(PortIntServantControlMask, ICW4_BASE);
}
