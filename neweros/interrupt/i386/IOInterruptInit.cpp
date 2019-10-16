#include <interrupt/Interrrupt.h>
#include <interrupt/i386/i386Interrupt.h>
#include <arch/i386/SystemTables.h>
#include <global/OS.h>

inline int getRoutineNoFromVecNo(int no) {
    return no - ClockVec;
}

void initIOInterrupt(PBYTE table) {
}