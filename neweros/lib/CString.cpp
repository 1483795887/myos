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

void CString::itos(int i, ULONG radix) {    //radix可能是10或是16
    int number = 1;
    int abs = i;
    int digit = 1;
	if (i < 0) {
		abs = -abs;
		if(radix != 16)
			appendChar('-');
	}
        
    while (number * radix <= abs) {
        number *= radix;
        digit++;
    }
    while (digit != 0) {
        int d = abs / number;
        char c;
        if (d < 10)
            c = d + '0';
        else
            c = d - 10 + 'a';
        appendChar(c);
        abs = abs % number;
        number /= radix;
		digit--;
    }


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
	delete buf;
	init(NULL);
    char** args = (char**)&fmt;
    char* p = (char*)fmt;
    int par = 1;
    CString* temp;
    BOOL translate = FALSE;
    while (*p) {
        if (*p == '%') {
            translate = TRUE;
            p++;
            continue;
        }
        if (translate) {
            switch (*p) {
            case 's':
                appendString(args[par]);
                break;
            case 'd':
                itos((int)args[par], 10);
                break;
			case 'x':
				itos((int)args[par], 16);
				break;
			case 't':
				temp = (CString*)args[par];
				appendString(temp->getBuf());
            default:
                break;
            }
            par++;
            translate = FALSE;
        } else
            appendChar(*p);

        p++;
    }
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
