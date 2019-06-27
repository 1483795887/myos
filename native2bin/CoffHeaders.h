#pragma once
#include <stdint.h>

typedef struct {
	uint16_t Machine;
	uint16_t NumOfSections;
	uint32_t Unknown;
	uint32_t PointerToSymbolTable;
	uint32_t NumOfSymbols;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
}IMAGE_FILE_HEADER;

typedef enum {
	IMAGE_SCN_LINK_REMOVE = 0x800,
	IMAGE_SCN_MEM_DISCARDABLE = 0x02000000
}SectionHeaderCharacteristics;

typedef struct {
	char Name[8];
	uint32_t VirtualSize;
	uint32_t RVA;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLineNumbers;
	uint16_t NumOfRelocations;
	uint16_t NumOfLineNumbers;
	uint32_t Characteristics;
}IMAGE_SECTION_HEADER;

typedef struct {
	union {
		char shortname[8];
		struct {
			uint32_t zero;
			uint32_t offsetIntoStringTable;
		}longname;
	};
	uint32_t value;
	uint16_t sectionNumber;
	uint16_t type;
	uint8_t  storageClass;
	uint8_t  numberOfAuxSymbols;

}SYMBOL;

typedef struct {
	uint32_t RVA;
	uint32_t SymbolTableIndex;
	uint16_t Type;
}IMAGE_RELOCATION;