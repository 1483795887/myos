#pragma once
#include <Types.h>
#include "Interrupter.h"
#include "TrapFrame.h"

class IRQ {
public:
	IRQ();
	void setIRQ(int irq);
	void bindInterrupter(Interrupter* interrupter);
	DWORD handle(TrapFrame* trapframe);
private:
	int irq;
	Interrupter* interrupter;
};