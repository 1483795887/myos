#pragma once

typedef enum {
	DivideZeroVec = 0,
	DebugVec,
	NMIVec,
	BreakPointVec,
	OverflowVec,
	BoundsVec,
	InvalidOpVec,
	DeviceNotAvailableVec,
	DoubleFaultVec,
	CoprocSegOverrunVec,
	InvalidTSSVec,
	trapSegNotPresentVec,
	StackFaultVec,
	ProtectionVec,
	PageFaultVec,
	CoprocErrVec = 0x10,

	ClockVec = 0x20,
	KeyboardVec = 0x21,
	MouseVec = 0x2c,
	HardDiskVec = 0x2e,
	LastIntVec	= 0x2f,

	SyscallVec	= 0x80
}InterruptVec;

#define MaxCountOfTraps CoprocErrVec
#define MaxCountOfIOs	0x16

