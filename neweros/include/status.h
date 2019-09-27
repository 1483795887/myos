#pragma once

enum Status {
	StatusSuccess = 0,
	StatusNotAligned,
	StatusMemoryOverLimit,
	StatusPageAlreadyExist,
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