#pragma once
#include "../Types.h"
#include "Rect.h"

class Container {
public:
	Container(Rect* rect); 
	void setRect(Rect* rect); 
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