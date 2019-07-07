#pragma once
#include "../types.h"

typedef struct {
	WORD magic;
	BYTE reserved[0x3a];
	DWORD lfanew;
}DosHeader;

typedef struct {
	DWORD signature;
	WORD  machine;
	WORD  numberOfSections;
	DWORD timeDateStamp;
	DWORD pointerToSymbolTable;
	DWORD numberOfSymbols;
	WORD  sizeOfOptionalHeader;
	WORD  characteristics;
}NtHeader;

typedef struct {
	WORD  magic;
	BYTE  majorLinkerVersion;
	BYTE  minorLinkerVersion;
	DWORD sizeOfCode;
	DWORD sizeOfInitializedData;
	DWORD sizeOfUninitializedData;
	DWORD addressOfEntryPoint;
	DWORD baseOfCode;
	DWORD baseOfData;
	DWORD imageBase;
	DWORD sectionAlignment;
	WORD  majorOperatingSystemVersion;
	WORD  minorOperatingSystemVersion;
	DWORD win32VersionValue;
	DWORD sizeOfImage;
	DWORD sizeOfHeaders;
	DWORD checkSum;
	WORD  subsystem;
	WORD  dllCharacteristics;
	DWORD sizeOfStackReserve;
	DWORD sizeOfStackCommit;
	DWORD sizeOfHeapReserve;
	DWORD sizeOfHeapCommit;
	DWORD loaderFlags;
	DWORD numberOfRvaAndSizes;
}OptionalHeader;

typedef struct {
	char  name[8];
	DWORD virtualSize;
	DWORD virtualAddress;
	DWORD rawSize;
	DWORD rawAddress;
	DWORD relocAddress;
	DWORD lineNumbers;
	DWORD relocationsNumber;
	DWORD lineNumbersNumber;
	DWORD characteristics;
}Section;

class PeFileParser {
private:
	DosHeader *header;
	BOOL valid;
public:
	PeFileParser(PVOID file);
	BOOL isFileValid();
};