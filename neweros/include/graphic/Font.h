#pragma once

#include "../Types.h"
#include "Graphic.h"

class Font {
public:
	Font() {
		color = DEFAULT_COLOR;
	}
    virtual void Draw(Graphic* graphic, char ch, ULONG x, ULONG y) {
    };
    ULONG getFontHeight() {
        return 1;
    }
    ULONG getFontWidth() {
        return 1;
    }
	void setColor(RGB rgb) {
		this->color = rgb;
	}
	RGB getColor() {
		return color;
	}
protected:
	RGB color;
};
