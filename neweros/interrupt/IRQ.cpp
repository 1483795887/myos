#include <interrupt/IRQ.h>
#include <Status.h>

IRQ::IRQ() {
    interrupter = NULL;
}

void IRQ::setIRQ(int irq) {
    this->irq = irq;
}

void IRQ::bindInterrupter(Interrupter * interrupter) {
	if (interrupter != NULL)
		this->interrupter = interrupter;
}

DWORD IRQ::handle(TrapFrame * trapframe) {
    if (trapframe == NULL)
        return StatusNullPointer;
    if (interrupter == NULL)
        return StatusNullPointer;
    return interrupter->handle(trapframe);
}
