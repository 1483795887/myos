#pragma once

typedef enum {
	Timer,
	Keyboard,
	Mouse,
	HardDisk
}IntType;

typedef void (_cdecl *InterruptRoutine)();