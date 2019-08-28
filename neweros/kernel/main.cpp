#include <global/OS.h>
#include <arch/CPU.h>
#include <arch/KernelTables.h>
#include <mm/BucketPool.h>
#include <lib/CString.h>

void initGdt() {
	PBYTE globalDescriptorTable = os->ppm->allocatePage(0);
	
	os->gdt.setTable(globalDescriptorTable);

	os->gdt.setDescriptor(KernelCode, 0, 4 * G - 1, Code, Kernel);
	os->gdt.setDescriptor(KernelData, 0, 4 * G - 1, Data, Kernel);
	os->gdt.setDescriptor(UserCode, 0, 4 * G - 1, Code, User);
	os->gdt.setDescriptor(UserData, 0, 4 * G - 1, Data, User);

	CPU::setGlobalDescriptorTable(globalDescriptorTable);
}

void main(OS* theOs) {
	os = theOs;

	initGdt();

	BucketPool* pool = New BucketPool;	//在naivepool中
	pool->setAllocator(os->allocator);
	os->pool = pool;					//从此开始堆就在bucketpool中

	CString str;
	for (int i = 0; i < 100; i++) {
		str.format("%d ", i);
		os->console->print(str.getBuf());
	}

    for (;;);
}
