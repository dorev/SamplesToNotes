#pragma once

// Set all FFT buffers sizes
#if !defined(AUDIO_FFT_WINDOW_SIZE)
	#define AUDIO_FFT_WINDOW_SIZE 16384
#endif

// FFTW primitive types can be float or double
#if !defined(FFTW_FLOAT) && !defined(FFTW_DOUBLE)
	#define FFTW_FLOAT
#endif

#if defined(FFTW_FLOAT) && defined(FFTW_DOUBLE)
#error FFTW_FLOAT and FFTW_DOUBLE cannot both be defined
#endif