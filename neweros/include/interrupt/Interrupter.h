#pragma once
#include <Types.h>
#include "TrapFrame.h"

class Interrupter{
public:
	virtual DWORD handle(TrapFrame* frame) = 0;
};