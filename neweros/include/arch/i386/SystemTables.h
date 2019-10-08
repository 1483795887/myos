#pragma once
#include <Types.h>
#include <interrupt/Interrrupt.h>

enum {
    SegDummpy = 0,
    SegKernelCode = 1,
    SegKernelData = 2,
    SegUserCode = 3,
    SegUserData = 4
};

typedef enum {
    SegTypeCode,
    SegTypeData,
    SegTypeTSS
} DescriptorType;

void setDescriptor(PBYTE table, ULONG n, PBYTE start, ULONG limit,
                          DescriptorType type, Privilege privilege);

void setTrapHandler(PBYTE table, int no, InterruptRoutine handler);
void setInterruptHandler(PBYTE table, int no, InterruptRoutine handler);
void setSyscallHandler(PBYTE table, InterruptRoutine handler);