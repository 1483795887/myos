#include <global/OS.h>

OS* os = NULL;

Status OS::getLastStatus() {
    return this->lastStatus;
}

void OS::setLastStatus(Status status) {
    this->lastStatus = status;
}
