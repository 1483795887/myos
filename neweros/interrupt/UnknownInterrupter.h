#pragma once
#include <interrupt/Interrupter.h>

class UnknownInterrupter : public Interrupter {
public:
    virtual DWORD handle(TrapFrame* trap);
};

