// native2bin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//使用这个的汇编要求只有一个节就是.text节

#include "pch.h"
#include <iostream>
#include <vector>
#include "CoffHeaders.h"

using namespace std;

IMAGE_SECTION_HEADER* findCodeSegment(char *pSections, IMAGE_FILE_HEADER* header) {
	char* pCurrentSection = pSections;
	IMAGE_SECTION_HEADER* result = NULL;
	for (int i = 0; i < header->NumOfSections; i++) {
		IMAGE_SECTION_HEADER* sectionHeader = (IMAGE_SECTION_HEADER*)pCurrentSection;
		pCurrentSection += sizeof(IMAGE_SECTION_HEADER);

		if (!strcmp(sectionHeader->Name, ".text") && sectionHeader->SizeOfRawData != 0)
			result = sectionHeader;
	}
	return result;
}

void handleRelocation(char* objFile, IMAGE_FILE_HEADER* header,IMAGE_SECTION_HEADER* codeSection) {
	char* codeData = objFile + codeSection->PointerToRawData;
	IMAGE_RELOCATION* relocations = (IMAGE_RELOCATION*)(objFile + codeSection->PointerToRelocations);
	SYMBOL* symbols = (SYMBOL*)(objFile + header->PointerToSymbolTable);
	for (int i = 0; i < codeSection->NumOfRelocations; i++) {
		IMAGE_RELOCATION* relocation = &relocations[i];
		int symbolIndex = relocation->SymbolTableIndex;
		char* pRelocation = objFile + codeSection->PointerToRawData + relocation->RVA;
		SYMBOL* symbol = &symbols[symbolIndex];

		uint32_t value = symbol->value + codeSection->RVA;

		*((uint32_t*)symbolIndex) = value;
	}
}

int main(int argc, char** argv) {
	if (argc != 4) {
		cout << "usage native2bin.exe *.obj *.bin base" << endl;
		return -1;
	}
        

	FILE *pfobj, *pfbin;

    string objfile = string(argv[1]);
    string binfile = string(argv[2]);

	uint32_t base;
	sscanf_s(argv[3], "%x", &base);

    fopen_s(&pfobj, objfile.c_str(), "r");
    fopen_s(&pfbin, binfile.c_str(), "w");

	fseek(pfobj, 0, SEEK_END);
    int fileSize = ftell(pfobj);
	fseek(pfobj, 0, SEEK_SET);

    char* objFile = (char*)malloc(fileSize);
    fread_s(objFile, fileSize, 1, fileSize, pfobj);
    fclose(pfobj);

	IMAGE_FILE_HEADER *header = (IMAGE_FILE_HEADER*)objFile;

	IMAGE_SECTION_HEADER* codeSection = findCodeSegment(objFile + sizeof(IMAGE_FILE_HEADER), header);

	codeSection->RVA = base;

	handleRelocation(objFile, header, codeSection);

	char* code = objFile + codeSection->PointerToRawData;
	int codeSize = codeSection->SizeOfRawData;

	fwrite(code, 1, codeSize, pfbin);

	free(objFile);
    fclose(pfbin);

}