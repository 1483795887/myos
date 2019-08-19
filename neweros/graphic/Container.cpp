#include <global/OS.h>
#include <graphic/Container.h>

Container::Container(Rect* rect) {
    this->rect = *rect;
    graphic = os->graphic->getSubGraphic(rect);
}

void Container::setRect(Rect* rect) {
    this->rect = *rect;
    graphic = os->graphic->getSubGraphic(rect);
}
