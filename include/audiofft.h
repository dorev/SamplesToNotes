#pragma once

#include <fftw3.h>
#include <vector>
#include "definitions.h"

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
    static const size_t InputBufferSize = N;
    static const size_t OutputBufferSize = N / 2 + 1;

    AudioFft(FftwReal samplingRate);
    ~AudioFft();
    FftwReal* GetInputBuffer();
    int GetInputBufferSize() const;
    FftwComplex* GetOutputBuffer() const;
    int GetOutputBufferSize() const;
    void Execute();
    void Execute(FftwReal* buffer);
    std::vector<BinFrequencyValue> GetHighestBins(size_t resultSize) const;
    const std::vector<FftwReal>& GetBinFrequencies() const;

private:
    FftwReal _samplingRate;
    FftwReal* _inputBuffer;
    FftwComplex* _outputBuffer;
    std::vector<float> _binCenterFrequencies;
    FftwPlan _plan;
};

}