#pragma once

#include "fftwutils.h"
#include "audiofft.h"

namespace SamplesToNotes
{

AudioFft::AudioFft(size_t windowSize, FftwReal samplingRate)
    : _windowSize(windowSize)
    , _samplingRate(samplingRate)
    , _plan(FftwDftPlan(_windowSize, _inputBuffer, _outputBuffer, FFTW_ESTIMATE))
    , _inputBuffer(nullptr)
    , _outputBuffer(nullptr)
    , _binCenterFrequencies(GetOutputBufferSize(), 0.0)
{
    // Allocate arrays
    _inputBuffer = reinterpret_cast<FftwReal*>(FftwMalloc(sizeof(FftwReal) * GetInputBufferSize()));
    _outputBuffer = reinterpret_cast<FftwComplex*>(FftwMalloc(sizeof(FftwComplex) * GetOutputBufferSize()));

    // Set bins center frequency values
    FftwReal binHzRange = samplingRate / GetInputBufferSize();
    for (size_t i = 0; i < GetOutputBufferSize(); ++i)
        _binCenterFrequencies[i] = (i * binHzRange) + (binHzRange / 2);
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
size_t AudioFft::GetInputBufferSize() const
{
    return _windowSize;
}

size_t AudioFft::GetOutputBufferSize() const
{
    return (_windowSize / 2) + 1;
}

FftwReal* AudioFft::GetInputBufferPointer()
{
    return _inputBuffer;
}

FftwComplex* AudioFft::GetOutputBufferPointer() const
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

BinFrequencyValue AudioFft::GetTopBin() const
{
    return GetTopBins(1)[0];
}

std::vector<BinFrequencyValue> AudioFft::GetTopBins(size_t topResults) const
{
    std::vector<BinFrequencyValue> results(topResults, {0, 0, 0});

    // Scan whole output buffer
    for (size_t bin = 0; bin < GetOutputBufferSize(); ++bin)
    {
        // Compare with each saved result
        for (size_t i = 0; i < topResults; ++i)
        {
            FftwReal value = _outputBuffer[bin][0];
            BinFrequencyValue& result = results[i];
            if (value > result.value)
            {
                // Shift right lower values
                for (size_t j = topResults - 1; j > i; --j)
                    results[j] = results[j - 1];

                // Insert higher result
                result.bin = bin;
                result.frequency = _binCenterFrequencies[bin];
                result.value = value;
                break;
            }
        }
    }
    return results;
}

const std::vector<FftwReal>& AudioFft::GetBinFrequencies() const
{
    return _binCenterFrequencies;
}

}