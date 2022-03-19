#pragma once

#include <fftw3.h>
#include <vector>
#include "defines.h"

namespace SamplesToNotes
{

struct BinFrequency
{
    FftwReal low;
    FftwReal center;
    FftwReal high;
};

struct BinData
{
    size_t number;
    FftwReal value;
    BinFrequency frequency;
};

class AudioFft
{
    
public:
    ONLY_EXPLICIT_CONSTRUCTOR(AudioFft);
    AudioFft(size_t windowSize, FftwReal samplingRate);
    ~AudioFft();
    FftwReal* GetInputBuffer();
    FftwComplex* GetOutputBuffer() const;
    void Execute();
    void Execute(FftwReal* buffer);
    BinData GetTopBin() const;
    std::vector<BinData> GetTopBins(size_t topResults = 3) const;
    const std::vector<BinFrequency>& GetBinFrequencies() const;
    size_t GetBinForFrequency(FftwReal frequency) const;
    size_t GetWindowSize() const;
    FftwReal GetBinRange() const;
    size_t GetInputBufferSize() const;
    size_t GetOutputBufferSize() const;

private:
    size_t _windowSize;
    FftwReal _samplingRate;
    FftwReal _binRange;
    FftwReal* _inputBuffer;
    FftwComplex* _outputBuffer;
    std::vector<BinFrequency> _binFrequencies;
    FftwPlan _plan;
};

}