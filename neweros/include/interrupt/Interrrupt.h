#pragma once
#include <Types.h>
#include "TrapFrame.h"

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

extern "C" void _cdecl commonIntHandler(PBYTE par);

void initInterrupt();
void initTrap(PBYTE table);
void initIOInterrupt(PBYTE table);
void initSyscall(PBYTE table);


DWORD DefaultInterruptHandler(TrapFrame* frame);