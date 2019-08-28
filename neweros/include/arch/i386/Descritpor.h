#pragma once
#include <types.h>

#define	DA_DPL0 0x00
#define DA_DPL1 0x20
#define DA_DPL2 0x40
#define DA_DPL3 0x60

#define	DA_DR			0x90	
#define	DA_DRW			0x92	
#define	DA_DRWA			0x93	
#define	DA_C			0x98	
#define	DA_CR			0x9A	
#define	DA_CCO			0x9C	
#define	DA_CCOR			0x9E	

#define	DA_LDT			0x82	
#define	DA_TaskGate		0x85	
#define	DA_386TSS		0x89	
#define	DA_386CGate		0x8C	
#define	DA_386IGate		0x8E	
#define	DA_386TGate		0x8F	

#define	DA_32			0x4000	
#define	DA_LIMIT_4K		0x8000	

class Descriptor {
public:
	Descriptor(ULONG start, ULONG limit, ULONG flags);
private:
	WORD limitLow;
	WORD baseLow;
	BYTE  baseMid;
	BYTE  attr1;
	BYTE  limitHighAttr2;
	BYTE  baseHigh;
};
