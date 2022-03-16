#pragma once

#include "fftwutils.h"
#include "audiofft.h"

namespace SamplesToNotes
{

AudioFft::AudioFft(FftwReal samplingRate)
    : _samplingRate(samplingRate)
    , _plan(FftwDftPlan(N, _inputBuffer, _outputBuffer, FFTW_ESTIMATE))
    , _inputBuffer(nullptr)
    , _outputBuffer(nullptr)
    , _binCenterFrequencies(OutputBufferSize, 0.0)
{
    // Allocate arrays
    _inputBuffer = reinterpret_cast<FftwReal*>(FftwMalloc(sizeof(FftwReal) * InputBufferSize));
    _outputBuffer = reinterpret_cast<FftwComplex*>(FftwMalloc(sizeof(FftwComplex) * OutputBufferSize));

    // Set bins center frequency values
    FftwReal binHzRange = samplingRate / InputBufferSize;
    for (size_t i = 0; i < OutputBufferSize; ++i)
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

FftwReal* AudioFft::GetInputBufferPointer()
{
    return _inputBuffer;
}

int AudioFft::GetInputBufferSize() const
{
    return InputBufferSize;
}

FftwComplex* AudioFft::GetOutputBufferPointer() const
{
    return _outputBuffer;
}

int AudioFft::GetOutputBufferSize() const
{
    return OutputBufferSize;
}

void AudioFft::Execute()
{
    HammingWindow(_inputBuffer, InputBufferSize);
    FftwExecuteDft(_plan, _inputBuffer, _outputBuffer);
}

void AudioFft::Execute(FftwReal* buffer)
{
    HammingWindow(buffer, InputBufferSize);
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
    for (size_t bin = 0; bin < OutputBufferSize; ++bin)
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