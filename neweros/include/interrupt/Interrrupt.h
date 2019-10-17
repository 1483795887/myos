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

extern "C" void _cdecl commonIntHandler(TrapFrame* frame);

void initInterrupt();
void initTrap(PBYTE table);
void initIOInterrupt(PBYTE table);
void initSyscall(PBYTE table);

#define MAX_IRQ 256