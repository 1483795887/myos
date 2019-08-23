#include <global/OS.h>


void main(OS* theOs) {
	os = theOs;
	os->console->print("Test");

    for (;;);
}
