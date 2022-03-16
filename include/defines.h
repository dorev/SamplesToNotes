#pragma once

#include "config.h"

#if defined(FFTW_FLOAT)
	#define _2PI 6.28318530718f
	#define _PI 3.14159265359f
	#define FFTW_REAL(x) x ## .0f
#elif defined(FFTW_DOUBLE)
	#define _2PI 6.28318530718
	#define _PI 3.14159265359
	#define FFTW_CAST(x) x ## .0
#endif

#define ONLY_EXPLICIT_CONSTRUCTOR(Class) \
	Class() = delete; \
	Class(const Class&) = delete; \
	Class(Class&&) = delete;
