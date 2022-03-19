#pragma once

#include "fftwutils.h"
#include "audiofft.h"

namespace SamplesToNotes
{

AudioFft::AudioFft(size_t windowSize, FftwReal samplingRate)
    : _windowSize(windowSize)
    , _samplingRate(samplingRate)
    , _binRange(samplingRate / static_cast<FftwReal>(GetInputBufferSize()))
    , _plan(FftwDftPlan(static_cast<int>(_windowSize), _inputBuffer, _outputBuffer, FFTW_ESTIMATE))
    , _inputBuffer(nullptr)
    , _outputBuffer(nullptr)
    , _binFrequencies(GetOutputBufferSize())
{
    // Allocate arrays
    _inputBuffer = reinterpret_cast<FftwReal*>(FftwMalloc(sizeof(FftwReal) * GetInputBufferSize()));
    _outputBuffer = reinterpret_cast<FftwComplex*>(FftwMalloc(sizeof(FftwComplex) * GetOutputBufferSize()));

    // Set bins center frequency values
    for (size_t i = 0; i < GetOutputBufferSize(); ++i)
    {
        _binFrequencies[i].low = i * _binRange;
        _binFrequencies[i].center = (i * _binRange) + (_binRange / 2);
        _binFrequencies[i].high = (i + 1) * _binRange;
    }
}

AudioFft::~AudioFft()
{
    if (_inputBuffer)
    {
        FftwFree(_inputBuffer);
        _inputBuffer = nullptr;
    }
    if (_outputBuffer)
    {
        FftwFree(_outputBuffer);
        _outputBuffer = nullptr;
    }
    FftwDestroyPlan(_plan);
}

size_t AudioFft::GetWindowSize() const
{
    return _windowSize;
}

FftwReal AudioFft::GetBinRange() const
{
    return _binRange;
}

size_t AudioFft::GetInputBufferSize() const
{
    return _windowSize;
}

size_t AudioFft::GetOutputBufferSize() const
{
    return (_windowSize / 2) + 1;
}

FftwReal* AudioFft::GetInputBuffer()
{
    return _inputBuffer;
}

FftwComplex* AudioFft::GetOutputBuffer() const
{
    return _outputBuffer;
}

void AudioFft::Execute()
{
    HammingWindow(_inputBuffer, GetInputBufferSize());
    FftwExecuteDft(_plan, _inputBuffer, _outputBuffer);
}

void AudioFft::Execute(FftwReal* buffer)
{
    HammingWindow(buffer, GetInputBufferSize());
    FftwExecuteDft(_plan, buffer, _outputBuffer);
}

size_t AudioFft::GetBinForFrequency(FftwReal frequency) const
{
    for (size_t bin = 0; bin < GetOutputBufferSize(); ++bin)
    {
        if (frequency >= _binFrequencies[bin].low
            && frequency < _binFrequencies[bin].high)
            return bin;
    }
    throw "Requested frequency is not within FFT range";
}

BinData AudioFft::GetTopBin() const
{
    return GetTopBins(1)[0];
}

std::vector<BinData> AudioFft::GetTopBins(size_t topResults) const
{
    std::vector<BinData> results(topResults, {0, 0, 0});

    // Scan whole output buffer
    for (size_t bin = 0; bin < GetOutputBufferSize(); ++bin)
    {
        // Compare with each saved result
        for (size_t i = 0; i < topResults; ++i)
        {
            FftwReal value = _outputBuffer[bin][0];
            BinData& result = results[i];
            if (value > result.value)
            {
                // Shift right lower values
                for (size_t j = topResults - 1; j > i; --j)
                    results[j] = results[j - 1];

                // Insert higher result
                result.number = bin;
                result.value = value;
                result.frequency = _binFrequencies[bin];
                break;
            }
        }
    }
    return results;
}

const std::vector<BinFrequency>& AudioFft::GetBinFrequencies() const
{
    return _binFrequencies;
}

}