#include <fs/pe.h>
#include "..\include\mm\physicalPage.h"

inline PDE getPDEIndex(ULONG address) {
    return address >> PDE_SHIFT;
}

inline PTE getPTEIndex(ULONG address) {
	address = address << PDE_BITS;
	address = address >> PDE_BITS;
    return address >> PTE_SHIFT;
}

inline BOOL checkPageAddressValid(ULONG address) {
	if(((ULONG)address) > 4 * G - K)
		return FALSE;
	return TRUE;
}

PhysicalPageManager::PhysicalPageManager(PD pd) {
    this->pd = pd;
}

PhysicalPageManager::PhysicalPageManager() {
    this->pd = (PD)FAILED;
}

void PhysicalPageManager::setPD(PD pd)
{
	this->pd = pd;
}

PDE PhysicalPageManager::getPDE(ULONG address) {
    if (!checkPageAddressValid((ULONG)pd))
        return FAILED;
    return pd[getPDEIndex(address)];
}

PTE PhysicalPageManager::getPTE( ULONG address) {
	if (!checkPageAddressValid((ULONG)pd))
		return FAILED;
	PDE pde = getPDE(address);
	if (!checkPageAddressValid((ULONG)pde))
		return FAILED;
	PT pt = (PT)(pde & -8);
    return pt[getPTEIndex(address)];
}
