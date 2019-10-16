#pragma once

#include "Interrrupt.h"

class InterruptController {
public:
    virtual ~InterruptController() = 0;
    virtual void EnableInterrupt(IntType type) = 0;
    virtual void DisableInterrupt(IntType type) = 0;
    virtual void init() = 0;
};