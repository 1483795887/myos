#pragma once
#include <Types.h>

typedef enum {
	DivideZero,
	BreakPoint,
	PageFault,
	Timer,
	Keyboard,
	Mouse,
	HardDisk,
	Syscall
}IntType;

typedef void(_cdecl *InterruptRoutine)( );


class InterruptVectorTable {
public:
	InterruptVectorTable(PBYTE table);
	void setTrapHandler(int no, InterruptRoutine handler);
	void setInterruptHandler(int no, InterruptRoutine handler);
	void setSyscallHandler(InterruptRoutine handler);
private:
	PBYTE table;
	int getArchIntNoFromIntType(IntType type);
};

extern "C" void _cdecl trapHandler(PBYTE par);
extern "C" void _cdecl interruptHandler(PBYTE par);
extern "C" void _cdecl syscallHandler(PBYTE par);


void initInterrupt();
void initTrap(InterruptVectorTable* table);
void initIOInterrupt(InterruptVectorTable* table);
void initSyscall(InterruptVectorTable* table);
