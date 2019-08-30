#include <interrupt/Interrrupt.h>
#include <interrupt/i386/i386Interrupt.h>

extern "C" void _cdecl trapDivideZero();
extern "C" void _cdecl trapDebug();
extern "C" void _cdecl trapNMI();
extern "C" void _cdecl trapBreakPoint();
extern "C" void _cdecl trapOverflow();
extern "C" void _cdecl trapBounds();
extern "C" void _cdecl trapInvalidOp();
extern "C" void _cdecl trapDeviceNotAvailable();
extern "C" void _cdecl trapDoubleFault();
extern "C" void _cdecl trapCoprocSegOverrun();
extern "C" void _cdecl trapInvalidTSS();
extern "C" void _cdecl trapSegNotPresent();
extern "C" void _cdecl trapStackFault();
extern "C" void _cdecl trapProtection();
extern "C" void _cdecl trapPageFault();
extern "C" void _cdecl trapCoprocErr();

void initTrap(InterruptVectorTable* ivt) {
	ivt->setTrapHandler(DivideZeroVec, trapDivideZero);
	ivt->setTrapHandler(DebugVec, trapDebug);
	ivt->setTrapHandler(NMIVec, trapNMI);
	ivt->setTrapHandler(BreakPointVec, trapBreakPoint);
	ivt->setTrapHandler(OverflowVec, trapOverflow);
	ivt->setTrapHandler(BoundsVec, trapBounds);
	ivt->setTrapHandler(InvalidOpVec, trapInvalidOp);
	ivt->setTrapHandler(DeviceNotAvailableVec, trapDeviceNotAvailable);
	ivt->setTrapHandler(DoubleFaultVec, trapDoubleFault);
	ivt->setTrapHandler(CoprocSegOverrunVec, trapCoprocSegOverrun);
	ivt->setTrapHandler(InvalidTSSVec, trapInvalidTSS);
	ivt->setTrapHandler(trapSegNotPresentVec, trapSegNotPresent);
	ivt->setTrapHandler(StackFaultVec, trapStackFault);
	ivt->setTrapHandler(ProtectionVec, trapProtection);
	ivt->setTrapHandler(PageFaultVec, trapPageFault);
	ivt->setTrapHandler(CoprocErrVec, trapCoprocErr);

}