#pragma once
#include "../Types.h"

#define PAGE_SIZE 0x1000
#define PAGE_MASK -PAGE_SIZE
#define LOG2_PAGE_SIZE 12

inline ULONG getPageSizeByOrder(ULONG order) {
    return 1 << (order + LOG2_PAGE_SIZE);
}

inline ULONG getOrderByPageSize(ULONG size) {
    return size >> (LOG2_PAGE_SIZE);
}