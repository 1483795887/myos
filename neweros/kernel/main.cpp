#include <global/OS.h>
#include <mm/BucketPool.h>
#include <lib/CString.h>

void main(OS* theOs) {
	os = theOs;

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
