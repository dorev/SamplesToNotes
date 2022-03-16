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
    ONLY_EXPLICIT_CONSTRUCTOR(AudioFft);
    AudioFft(size_t windowSize, FftwReal samplingRate);
    ~AudioFft();
    FftwReal* GetInputBufferPointer();
    FftwComplex* GetOutputBufferPointer() const;
    void Execute();
    void Execute(FftwReal* buffer);
    BinFrequencyValue GetTopBin() const;
    std::vector<BinFrequencyValue> GetTopBins(size_t topResults = 3) const;
    const std::vector<FftwReal>& GetBinFrequencies() const;
    size_t GetWindowSize() const;
    size_t GetInputBufferSize() const;
    size_t GetOutputBufferSize() const;

private:
    size_t _windowSize;
    FftwReal _samplingRate;
    FftwReal* _inputBuffer;
    FftwComplex* _outputBuffer;
    std::vector<float> _binCenterFrequencies;
    FftwPlan _plan;
};

}