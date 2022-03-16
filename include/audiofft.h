#pragma once

#include <fftw3.h>
#include <vector>
#include "defines.h"

namespace SamplesToNotes
{

struct BinFrequencyValue
{
    size_t bin;
    FftwReal frequency;
    FftwReal value;
};

class AudioFft
{
public:
    static const size_t N = AUDIO_FFT_WINDOW_SIZE;
    static const size_t WindowSize = AUDIO_FFT_WINDOW_SIZE;
    static const size_t InputBufferSize = N;
    static const size_t OutputBufferSize = N / 2 + 1;
    
public:
    ONLY_EXPLICIT_CONSTRUCTOR(AudioFft);
    AudioFft(FftwReal samplingRate);
    ~AudioFft();
    FftwReal* GetInputBufferPointer();
    int GetInputBufferSize() const;
    FftwComplex* GetOutputBufferPointer() const;
    int GetOutputBufferSize() const;
    void Execute();
    void Execute(FftwReal* buffer);
    BinFrequencyValue GetTopBin() const;
    std::vector<BinFrequencyValue> GetTopBins(size_t topResults = 3) const;
    const std::vector<FftwReal>& GetBinFrequencies() const;

private:
    FftwReal _samplingRate;
    FftwReal* _inputBuffer;
    FftwComplex* _outputBuffer;
    std::vector<float> _binCenterFrequencies;
    FftwPlan _plan;
};

}