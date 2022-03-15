#include <cmath>
#include "utils.h"

namespace SamplesToNotes
{

FftwReal FrequencyOfNote(size_t note, FftwReal a4Frequency)
{
    static const FftwReal a = pow(FFTW_REAL(2), (FFTW_REAL(1)/ FFTW_REAL(12)));
    FftwReal delta = static_cast<FftwReal>(note - 57); // A4
    return a4Frequency * pow(a, delta);
}

FftwReal DifferencePercentage(FftwReal value1, FftwReal value2)
{
    return abs(value1 - value2) / ((value1 + value2) / FFTW_REAL(2)) * FFTW_REAL(100);
}

void GenerateSinWave(FftwReal* buffer, int bufferSize, FftwReal frequency, FftwReal samplingRate)
{
    FftwReal sampleDurationSeconds = FFTW_REAL(1) / samplingRate;
    for (int i = 0; i < bufferSize; ++i)
    {
        FftwReal sampleTime = i * sampleDurationSeconds;
        buffer[i] = sin(_2PI * frequency * sampleTime);
    }
}

}