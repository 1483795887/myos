#pragma once

#include "../types.h"

SIZE _cdecl strlen(const char* str);

class CString {
public:
    CString(const char* str);
	CString();
	~CString();
    char* getBuf();
	int getLen();
	void _cdecl format(const char* fmt, ...);

	void appendChar(char str);
	void appendString(char* str);

	CString& operator =(CString& str);
	CString& operator =(const char* str);
	CString operator +(CString& str);
	CString operator +(char* str);
private:
	void sizeTwice();
	void init(const char* str);

	void itos(int i, ULONG radix);

    char* buf;
    int len;
    int maxLen;
};