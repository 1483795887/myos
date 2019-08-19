#pragma once
#include "../Types.h"

#pragma pack(1)
#define DOS_MAGIC 0x5a4d
typedef struct {
	WORD magic;
	BYTE reserved[0x3a];
	DWORD lfanew;
}DosHeader;

#define SECTION_FLAG_CONTAIN_CODE				0x20
#define SECTION_FLAG_CONTAIN_INITAILIZED_DATA	0x40
#define SECTION_FLAG_EXECUTABLE					0x20000000
#define SECTION_FLAG_READABLE					0x40000000
#define SECTION_FLAG_WRITABLE					0x80000000


#define NT_SIGNATURE 0x4550
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
	WORD relocationsNumber;
	WORD lineNumbersNumber;
	DWORD characteristics;
}Section;

#pragma pack()