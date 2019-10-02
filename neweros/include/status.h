#pragma once

enum Status {
	StatusSuccess = 0,
	StatusSuccessButNoEffect,
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