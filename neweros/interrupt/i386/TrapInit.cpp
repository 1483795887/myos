#include <interrupt/Interrrupt.h>
#include <interrupt/i386/i386Interrupt.h>
#include <arch/i386/SystemTables.h>

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

void initTrap(PBYTE table) {
	setTrapHandler(table,DivideZeroVec, trapDivideZero);
	setTrapHandler(table,DebugVec, trapDebug);
	setTrapHandler(table,NMIVec, trapNMI);
	setTrapHandler(table, BreakPointVec, trapBreakPoint);
	setTrapHandler(table, OverflowVec, trapOverflow);
	setTrapHandler(table, BoundsVec, trapBounds);
	setTrapHandler(table, InvalidOpVec, trapInvalidOp);
	setTrapHandler(table, DeviceNotAvailableVec, trapDeviceNotAvailable);
	setTrapHandler(table, DoubleFaultVec, trapDoubleFault);
	setTrapHandler(table, CoprocSegOverrunVec, trapCoprocSegOverrun);
	setTrapHandler(table, InvalidTSSVec, trapInvalidTSS);
	setTrapHandler(table, trapSegNotPresentVec, trapSegNotPresent);
	setTrapHandler(table, StackFaultVec, trapStackFault);
	setTrapHandler(table, ProtectionVec, trapProtection);
	setTrapHandler(table, PageFaultVec, trapPageFault);
	setTrapHandler(table, CoprocErrVec, trapCoprocErr);

}