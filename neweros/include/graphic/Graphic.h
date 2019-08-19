#pragma once
#include "../Types.h"
#include "../Status.h"
#include "Rect.h"

typedef struct {
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE reserved;
} RGB;

#define BLACK {0,0,0,0}
#define WHITE {255,255,255}
#define DEFAULT_COLOR BLACK

class Graphic {
public:
    Status init(Rect* rect, PBYTE vam);
    Status putPixel(ULONG x, ULONG y, RGB rgb);
    RGB getPixel(ULONG x, ULONG y);
    Graphic getSubGraphic(Rect* rect);
    void setFather(Graphic* father);
private:
    Rect rect;
    PBYTE vam;

    Graphic* father;

    BOOL isPositionValid(ULONG x, ULONG y);
};