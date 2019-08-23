#pragma once

enum Status {
	Success = 0,
	AddressNotAligned,
	MemoryOverLimit,
	PageAlreadyExist,       //映射页面时页面已存在
	NullPointer,
	PoolNotEnough,
	ValueNotInRange,
	FreePageNotEnough,
	InvalidPage,
	PageAlreadyFreeed,
	ArrayBounds,
	SizeTooBig,
	ParameterError
};