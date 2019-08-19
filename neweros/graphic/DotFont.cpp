#include <graphic/DotFont.h>
#include <global/OS.h>

DotFont::DotFont(PBYTE map, ULONG width, ULONG height) {
	this->map = map;
	this->width = width;
	this->height = height;
}

void DotFont::Draw(Graphic* graphic, char ch, ULONG x, ULONG y) {
	PBYTE dots = &map[ch * width * height / 8];//一个字节八个位，每个位表示是否显示
	for (ULONG dy = 0; dy < height; dy++) {
		for (ULONG dx = 0; dx < width; dx++) {
			if ((dots[dy] << dx) & 0x80) {
				graphic->putPixel(x + dx, y + dy, color);
			}
		}
	}
}
