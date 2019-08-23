#pragma once
#include "../Types.h"

#define PAGE_SIZE 0x1000
#define PAGE_MASK -PAGE_SIZE
#define LOG2_PAGE_SIZE 12
#define KERNEL_BASE 0xc0000000

inline ULONG getPAFromVA(ULONG va) {
	return va - KERNEL_BASE;
}

inline ULONG getVAFromPA(ULONG pa) {
	return pa + KERNEL_BASE;
}

inline ULONG getPageSizeByOrder(ULONG order) {
    return 1 << (order + LOG2_PAGE_SIZE);
}

inline ULONG getOrderByPageSize(ULONG size) {
    return size >> (LOG2_PAGE_SIZE + 1);
}