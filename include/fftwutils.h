#pragma once

#include <fftw3.h>
#include "defines.h"

namespace SamplesToNotes
{

#if defined(FFTW_FLOAT)
	using FftwComplex = fftwf_complex;
	using FftwReal = float;
	using FftwPlan = fftwf_plan;
	#define FftwFree fftwf_free
	#define FftwMalloc fftwf_malloc
	#define FftwDftPlan fftwf_plan_dft_r2c_1d
	#define FftwDestroyPlan fftwf_destroy_plan
	#define FftwExecute fftwf_execute
	#define FftwExecuteDft fftwf_execute_dft_r2c
#elif defined(FFTW_DOUBLE)
	using FftwComplex = fftw_complex;
	using FftwReal = double;
	using FftwPlan = fftw_plan;
	#define FftwFree fftw_free
	#define FftwMalloc fftw_malloc
	#define FftwDftPlan fftw_plan_dft_r2c_1d
	#define FftwDestroyPlan fftw_destroy_plan
	#define FftwExecute fftw_execute
	#define FftwExecuteDft fftw_execute_dft_r2c
#else
	#error FFTW_FLOAT or FFTW_DOUBLE must be defined
#endif

void HammingWindow(FftwReal* buffer, size_t bufferSize);
void HannWindow(FftwReal* buffer, size_t bufferSize);
void ScaleToDecibel(FftwComplex* buffer, size_t bufferSize);

}