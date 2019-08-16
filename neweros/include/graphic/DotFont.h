#pragma once
#include "Font.h"

#define FONT_ADDRESS

class DotFont : public Font {
public:
    DotFont(PBYTE map, ULONG width, ULONG height);
    virtual void Draw(Graphic* graphic, char ch, ULONG x, ULONG y);
private:
    RGB rgb;
    PBYTE map;
    ULONG width;
    ULONG height;
};