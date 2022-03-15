#include <cmath>
#include "fftwutils.h"

namespace SamplesToNotes
{

void HammingWindow(FftwReal* buffer, size_t bufferSize)
{
    static const FftwReal a0 = FFTW_REAL(25) / FFTW_REAL(46);
    static const FftwReal a1 = FFTW_REAL(1) - a0;
    for (size_t i = 0; i < bufferSize; ++i)
        buffer[i] *= a0 - a1 * cos(_2PI * i / bufferSize);
}

void HannWindow(FftwReal* buffer, size_t bufferSize)
{
    for (size_t i = 0; i < bufferSize; ++i)
        buffer[i] = pow(sin((_PI * buffer[i]) / bufferSize), FFTW_REAL(2));
}

void ScaleToDecibel(FftwComplex* buffer, size_t bufferSize)
{
    for (size_t i = 0; i < bufferSize; ++i)
        buffer[i][0] = FFTW_REAL(10) * log10(buffer[i][0] * buffer[i][0]);
}

}