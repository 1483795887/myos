#pragma once

#include "Container.h"
#include "Font.h"

class Console :public Container {
public:
	Console(Rect* rect):Container(rect) {
		col = 0;
		row = 0;
	}
	void print(char* str);
	void setRow(ULONG row);
	void setCol(ULONG col);
	ULONG getRow() {
		return row;
	}
	ULONG getCol() {
		return col;
	}
	void setFont(Font* font) {
		this->font = font;
	}
	Font* getFont() {
		return font;
	}
private:
	ULONG row;	//行
	ULONG col;	//列
	Font* font;
};