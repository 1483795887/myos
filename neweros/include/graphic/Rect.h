#pragma once
#include "../Types.h"

class Rect {
public:
    Rect(ULONG x, ULONG y, ULONG width, ULONG height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
	Rect() {
		x = 0;
		y = 0;
		width = 1;
		height = 1;
	}

    ULONG x;
    ULONG y;
    ULONG width;
    ULONG height;
};