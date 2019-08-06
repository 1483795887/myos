#include <global/OS.h>
#include <mm/Mm.h>
#include <mm/Zone.h>
#include <mm/Page.h>

Status FreeArea::init(ULONG order, ULONG memorySize) {
    Status status;
    if (order > MAX_ORDER) {
        status = ValueNotInRange;
        os->setLastStatus(status);
        return status;
    }
    this->order = order;

    map.init(memorySize >> (order + LOG2_PAGE_SIZE));
    return os->getLastStatus();
}

Status Zone::init(PBYTE start, ULONG memorySize) {
    return os->getLastStatus();
}

ULONG Zone::getFreePages() {
    return 0;
}
