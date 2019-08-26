#include <global/OS.h>
#include <lib/Memory.h>
#include <lib/CString.h>

void CString::init(const char* str) {
    maxLen = 16;
    len = strlen(str);
    while (maxLen < len)
        maxLen <<= 1;
    buf = New char[maxLen];
    memcpy(buf, str, len);
}

CString::CString(const char* str) {
    init(str);
}

CString::CString() {
    init(NULL);
}

CString::~CString() {
    delete buf;
    buf = NULL;
}

char* CString::getBuf() {
    return this->buf;
}

int CString::getLen() {
    return len;
}

void CString::format(const char* fmt, ...) {

}

void CString::appendChar(char ch) {
    if (len + 1 > maxLen)
        sizeTwice();
    buf[len++] = ch;
}

void CString::appendString(char* str) {
	int size = strlen(str);
    while (maxLen < len + size && maxLen < PAGE_SIZE)
        sizeTwice();
    memcpy(buf + len, str, size);
    len += size;
}

CString& CString::operator=(CString & str) {
    if (str.getBuf() != buf) {
        delete buf;
        init(str.getBuf());
    }
    return *this;
}

CString& CString::operator=(const char* str) {
    if (str != buf) {
        delete buf;
        init(str);
    }
    return *this;
}

CString CString::operator+(CString & str) {
    CString result(buf);
    result.appendString(str.getBuf());
    return result;
}

CString CString::operator+(char* str) {
    CString result(buf);
    result.appendString(str);
    return result;
}

void CString::sizeTwice() {
    if ((maxLen << 1) >= PAGE_SIZE)
        return;
    char* newbuf = New char[maxLen * 2];
	memcpy(newbuf, buf, maxLen);
	maxLen <<= 1;
	delete buf;
	buf = newbuf;
}

SIZE _cdecl strlen(const char* str) {
    if (str == NULL)
        return 0;
    int len = 0;
    for (len = 0; str[len] != 0; len++) {}
    return len;
}
