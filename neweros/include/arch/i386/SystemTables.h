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

typedef void(_cdecl *InterruptEntry)();

void setDescriptor(PBYTE table, ULONG n, PBYTE start, ULONG limit,
                          DescriptorType type, Privilege privilege);

void setTrapHandler(PBYTE table, int no, InterruptEntry handler);
void setInterruptHandler(PBYTE table, int no, InterruptEntry handler);
void setSyscallHandler(PBYTE table, InterruptEntry handler);