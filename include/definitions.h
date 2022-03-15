#pragma once

#if !defined(AUDIO_FFT_WINDOW_SIZE)
	#define AUDIO_FFT_WINDOW_SIZE 16384
#endif

#if !defined(FFTW_FLOAT) && !defined(FFTW_DOUBLE)
	#define FFTW_FLOAT
#endif

#if defined(FFTW_FLOAT) && defined(FFTW_DOUBLE)
	#error FFTW_FLOAT and FFTW_DOUBLE cannot both be defined
#endif

#if defined(FFTW_FLOAT)
	#define _2PI 6.28318530718f
	#define _PI 3.14159265359f
	#define FFTW_REAL(x) x ## .0f
#elif defined(FFTW_DOUBLE)
	#define _2PI 6.28318530718
	#define _PI 3.14159265359
	#define FFTW_CAST(x) x ## .0
#endif