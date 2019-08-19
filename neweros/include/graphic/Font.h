#pragma once

#include "../Types.h"
#include "Graphic.h"

class Font {
public:
	Font() {
		color = DEFAULT_COLOR;
		width = 1;
		height = 1;
	}
    virtual void Draw(Graphic* graphic, char ch, ULONG x, ULONG y) {
    };
    ULONG getFontHeight() {
        return height;
    }
    ULONG getFontWidth() {
        return width;
    }
	void setColor(RGB rgb) {
		this->color = rgb;
	}
	RGB getColor() {
		return color;
	}
protected:
	RGB color;
	ULONG width;
	ULONG height;
};
