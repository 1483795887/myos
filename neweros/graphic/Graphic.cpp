#include <graphic/Graphic.h>
#include <global/OS.h>

Status Graphic::init(Rect* rect, PBYTE vam) {
    this->rect = *rect;
    this->vam = vam;
	this->father = NULL;
    if (vam == NULL)
        os->setLastStatus(NullPointer);
    if (rect->width == 0 || rect->height == 0)
        os->setLastStatus(ValueNotInRange);
    return os->getLastStatus();
}

Status Graphic::putPixel(ULONG x, ULONG y, RGB rgb) {
    if (!isPositionValid(x, y))
        return ArrayBounds; //不会对系统产生任何影响，也不用检测这个，只是为了方便测试
	if (father != NULL)
		return father->putPixel(x + rect.x, y + rect.y, rgb);
    ULONG offset = (y * rect.width + x) * 3;
    PBYTE data = vam + offset;
    data[0] = rgb.b;
    data[1] = rgb.g;
    data[2] = rgb.r;
    return Success;
}

RGB Graphic::getPixel(ULONG x, ULONG y) {
    if (!isPositionValid(x, y))
        return DEFAULT_COLOR;
	if (father != NULL)
		return father->getPixel(x + rect.x, y + rect.y);
    RGB rgb;
    ULONG offset = ((y + rect.y) * rect.width + (x + rect.x)) * 3;
    PBYTE data = vam + offset;
    rgb.b = data[0];
    rgb.g = data[1];
    rgb.r = data[2];
    return rgb;
}

Graphic Graphic::getSubGraphic(Rect* rect) {
    Graphic graphic;
    graphic.init(rect, vam);
	graphic.setFather(this);
    return graphic;
}

void Graphic::setFather(Graphic* father) {
    this->father = father;
}

BOOL Graphic::isPositionValid(ULONG x, ULONG y) {
    return (BOOL)(x < rect.width && y < rect.height);
}
