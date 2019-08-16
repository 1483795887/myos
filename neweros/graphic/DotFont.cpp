#include <graphic/DotFont.h>
#include <global/OS.h>

DotFont::DotFont(PBYTE map, ULONG width, ULONG height) {
	this->map = map;
	this->width = width;
	this->height = height;
}

void DotFont::Draw(Graphic* graphic, char ch, ULONG x, ULONG y) {
	PBYTE dots = &map[ch * width * height / 8];//一个字节八个位，每个位表示是否显示
	for (ULONG dy = 0; y < height; y++) {
		for (ULONG dx = 0; x < width; x++) {
			if ((dots[dy] & (1 << dx))) {
				graphic->putPixel(x + dx, y + dy, color);
			}
		}
	}
}
