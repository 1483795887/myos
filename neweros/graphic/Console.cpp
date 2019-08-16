#include <global/OS.h>
#include <graphic/Console.h>

void Console::print(char* str) {
    ULONG x, y;
    for (int i = 0; str[i] != 0; i++) {
        x = col * font->getFontWidth();
        y = row * font->getFontHeight();
        if (x >= rect.width || y >= rect.height)
            continue;
        font->Draw(&graphic, str[i], x, y);
        col++;
        if (x + font->getFontWidth() >= rect.width) {
            col = 0;
            row++;
        }
    }
}

void Console::setRow(ULONG row) {
    this->row = row;
}

void Console::setCol(ULONG col) {
    this->col = col;
}
