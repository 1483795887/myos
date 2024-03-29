#include "pch.h"
#include <mm/Mm.h>
#include <Types.h>

ULONG pageAlign(ULONG address) {
    return ulAlign(address, PAGE_SIZE, TRUE);
}
TEST(MMTest, pageAlignWhenAddressIsAlignedThenSame) {
    ULONG address = PAGE_SIZE * 3;
    EXPECT_EQ(pageAlign(address), address);
}

TEST(MMTest, pageUnalignWhenAddressIsAlignedThenPageUp) {
    ULONG address = PAGE_SIZE * 3;
    ULONG unAlignedAddress = address + 0x41;
    EXPECT_EQ(pageAlign(unAlignedAddress), address + PAGE_SIZE);
}