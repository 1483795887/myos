#pragma once
#include <graphic/Font.h>

class FakeFont: public Font {
public:
    FakeFont(ULONG width, ULONG height) {
        int size = (width + 1) * (height + 1);//多出一行一列用于测试
        this->width = width;
        this->height = height;
        this->buffer = new char[size];
        memset(buffer, 0, size);
    };
    ~FakeFont() {
        delete buffer;
    }
    virtual void Draw(Graphic* graphic, char ch, ULONG x, ULONG y);

    char* getString(ULONG x, ULONG y);

private:
    char* buffer;
    ULONG width;
    ULONG height;
};

