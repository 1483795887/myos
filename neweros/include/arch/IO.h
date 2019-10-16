#pragma once

#include <Types.h>

typedef WORD Port;

extern BYTE  _cdecl In8(Port port);
extern WORD  _cdecl In16(Port port);
extern DWORD _cdecl In32(Port port);

extern void _cdecl Out8(Port port, BYTE value);
extern void _cdecl Out16(Port port, WORD value);
extern void _cdecl Out32(Port port, DWORD value);