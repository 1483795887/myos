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

extern "C" void _cdecl commonIntHandler(PBYTE par);

void initInterrupt();
void initTrap(PBYTE table);
void initIOInterrupt(PBYTE table);
void initSyscall(PBYTE table);
