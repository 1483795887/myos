#include <fs/pe.h>

PeFileParser::PeFileParser(PVOID file) {
	this->header = (DosHeader*)file;
}

BOOL PeFileParser::isFileValid() {
	BOOL result = TRUE;
	return result;
}