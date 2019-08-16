#pragma once
#include "../Types.h"
#include <global/OS.h>
#include "Rect.h"

class Container {
public:
    Container(Rect* rect) {
        this->rect = *rect;
		graphic = os->graphic->getSubGraphic(rect);
    }
    void setRect(Rect* rect) {
        this->rect = *rect;
		graphic = os->graphic->getSubGraphic(rect);
    }
    Rect* getRect() {
        return &rect;
    }
	void setGraphic(Graphic* graphic) {
		this->graphic = *graphic;
	}
	Graphic* getGraphic() {
		return &graphic;
	}
protected:
    Rect rect;
	Graphic graphic;
};