#include "pch.h"
#include "FakeFont.h"

void FakeFont::Draw(Graphic* graphic, char ch, ULONG x, ULONG y) {
    graphic->putPixel(x, y, color);
    buffer[y * width + x] = ch;
}

char* FakeFont::getString(ULONG x, ULONG y) {
    return &buffer[y * width + x];
}
