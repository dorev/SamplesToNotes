#include "samplestonotes.h"

namespace SamplesToNotes
{

SamplesToNotes::SamplesToNotes(size_t fftWindowSize, FftwReal samplingRate, FftwReal a4Frequency)
	: _fftWindowSize(fftWindowSize)
	, _samplingRate(samplingRate)
	, _a4Frequency(a4Frequency)
	, _audioFft(_fftWindowSize, _samplingRate)
	, _noteFftInfo(_fftWindowSize, _samplingRate, _a4Frequency)
	, _fftOutputBuffer(_audioFft.GetOutputBufferPointer())
{
}

size_t SamplesToNotes::GetFftWindowSize() const
{
	return _fftWindowSize;
}

std::vector<NoteValue> SamplesToNotes::GetNotesFromSamples(FftwReal* sampleBuffer, size_t bufferSize, size_t resultSize)
{
	if (bufferSize < _fftWindowSize)
		throw "bufferSize should be greater or equal to the fftWindowSize";

	_audioFft.Execute(sampleBuffer);
		
	// Scan note bins from the FFT output buffer
	std::vector<NoteValue> results(resultSize, {0, 0});
	const std::vector<size_t>& noteBins = _noteFftInfo.GetNoteFftBins();
	for (size_t note = 0; note < NoteFftInfo::NoteLimit; ++note)
	{
		// Compare with each saved result
		for (size_t i = 0; i < resultSize; ++i)
		{
			FftwReal value = _fftOutputBuffer[noteBins[note]][0];
			NoteValue& result = results[i];
			if (value > result.value)
			{
				// Shift right lower values
				for (size_t j = resultSize - 1; j > i; --j)
					results[j] = results[j - 1];

				// Insert higher result
				result.note = note;
				result.value = value;
				break;
			}
		}
	}
	return results;
}

}