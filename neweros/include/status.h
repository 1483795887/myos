#pragma once

enum Status {
	StatusSuccess = 0,
	StatusAddressNotAligned,
	StatusMemoryOverLimit,
	StatusPageAlreadyExist,       //映射页面时页面已存在
	StatusNullPointer,
	StatusPoolNotEnough,
	StatusValueNotInRange,
	StatusFreePageNotEnough,
	StatusInvalidPage,
	StatusPageAlreadyFreeed,
	StatusArrayBounds,
	StatusSizeTooBig,
	StatusParameterError
};